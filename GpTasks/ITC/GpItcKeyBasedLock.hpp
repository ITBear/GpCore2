#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>
#include <GpCore2/GpTasks/ITC/GpItcLock.hpp>
#include <GpCore2/Config/IncludeExt/unordered_dense.hpp>

#include <functional>

namespace GPlatform {

template<typename KeyT>
class GpItcKeyBasedLock
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcKeyBasedLock)
    CLASS_DD(GpItcKeyBasedLock<KeyT>)
    TAG_SET(THREAD_SAFE)

    using key_type  = KeyT;

private:
    class ItcLocksPool;
    class ShardOfLocks;
    class Shards;

    // ---------------- ItcLocksPool Pool ----------------
    class ItcLocksPool
    {
        CLASS_REMOVE_CTRS_MOVE_COPY(ItcLocksPool)

        using PoolT = std::vector<GpItcLock::SP>;

    public:
                        ItcLocksPool    (void) noexcept;
                        ~ItcLocksPool   (void) noexcept;

        void            Init            (size_t aInitSize);
        GpItcLock::SP   Acquire         (void);
        void            Release         (GpItcLock::SP aItcLock);

    private:
        PoolT           iPool;
    };

    // ---------------- Shard of locks ----------------
    class ShardOfLocks
    {
        CLASS_REMOVE_CTRS_MOVE_COPY(ShardOfLocks)

        using LocksMapT = ankerl::unordered_dense::map<KeyT, std::tuple<GpItcLock::SP, size_t/*acquire counter*/>>;

    public:
                            ShardOfLocks    (void) noexcept = default;
                            ~ShardOfLocks   (void) noexcept = default;

        void                Init            (const size_t aItcLocksPoolInitSize);

        template<typename K>
        GpItcLock*          Acquire         (K&& aKey) noexcept;

        template<typename K>
        [[nodiscard]] bool  Release         (K&& aKey) noexcept;

    private:
        GpSpinLock          iSpinLock;
        LocksMapT           iLocksMap       GUARDED_BY(iSpinLock);
        ItcLocksPool        iItcLocksPool   GUARDED_BY(iSpinLock);
    };

    // ---------------- Shards ----------------
    class Shards
    {
        CLASS_REMOVE_CTRS_MOVE_COPY(Shards)

        using ShardLocksT = boost::container::small_vector<std::unique_ptr<ShardOfLocks>, 8>;

    public:
                            Shards      (void) noexcept = default;
                            Shards      (size_t aShardsCount,
                                         size_t aItcLocksPoolInitSize);
                            ~Shards     (void) noexcept = default;

        void                Init        (size_t aShardsCount,
                                         size_t aItcLocksPoolInitSize);

        template<typename K>
        GpItcLock*          Acquire     (K&& aKey) noexcept;

        template<typename K>
        [[nodiscard]] bool  Release     (K&& aKey) noexcept;

    private:
        ShardLocksT         iShards;
    };

    // ---------------- GpItcKeyBasedLock ----------------
public:
                        GpItcKeyBasedLock   (void) noexcept = default;
                        GpItcKeyBasedLock   (size_t aShardsCount,
                                             size_t aItcLocksPoolInitSize);
                        ~GpItcKeyBasedLock  (void) noexcept = default;

    void                Init                (size_t aShardsCount,
                                             size_t aItcLocksPoolInitSize);

    template<typename K>
    GpItcLock*          AcquireLock         (K&& aKey) noexcept;

    template<typename K>
    [[nodiscard]] bool  ReleaseLock         (K&& aKey) noexcept;

private:
    Shards              iShards;
};

// ---------------- ItcLocksPool Pool ----------------

template<typename KeyT>
GpItcKeyBasedLock<KeyT>::ItcLocksPool::ItcLocksPool (void) noexcept
{
}

template<typename KeyT>
GpItcKeyBasedLock<KeyT>::ItcLocksPool::~ItcLocksPool (void) noexcept
{
}

template<typename KeyT>
void    GpItcKeyBasedLock<KeyT>::ItcLocksPool::Init (const size_t aInitSize)
{
    iPool.resize(aInitSize);

    for (size_t id = 0; id < aInitSize; id++)
    {
        iPool[id] = MakeSP<GpItcLock>();
    }
}

template<typename KeyT>
GpItcLock::SP   GpItcKeyBasedLock<KeyT>::ItcLocksPool::Acquire (void)
{
    if (iPool.empty()) [[unlikely]]
    {
        iPool.emplace_back(MakeSP<GpItcLock>());
    }

    // Extract lock from iPool
    GpItcLock::SP itcLockSP = std::move(iPool.back());
    iPool.pop_back();

    return itcLockSP;
}

template<typename KeyT>
void    GpItcKeyBasedLock<KeyT>::ItcLocksPool::Release (GpItcLock::SP aItcLock)
{
    iPool.emplace_back(std::move(aItcLock));
}

// ---------------- Shard of locks ----------------

template<typename KeyT>
void    GpItcKeyBasedLock<KeyT>::ShardOfLocks::Init (const size_t aItcLocksPoolInitSize)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    iItcLocksPool.Init(aItcLocksPoolInitSize);
}

template<typename KeyT>
template<typename K>
GpItcLock*  GpItcKeyBasedLock<KeyT>::ShardOfLocks::Acquire (K&& aKey) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    GpItcLock*  itcLock = nullptr;
    auto        iter    = iLocksMap.find(KeyT{aKey});

    if (iter != iLocksMap.end())
    {
        auto&[itcLockSP, acquireCounter] = iter->second;
        acquireCounter++;
        itcLock = itcLockSP.Pn();
    } else
    {
        // Key not found, get from pool
        try
        {
            auto&[itcLockSP ,_] = iLocksMap.emplace
            (
                std::forward<K>(aKey),
                std::tuple<GpItcLock::SP, size_t>
                {
                    iItcLocksPool.Acquire(),
                    size_t{1} // Acquire counter
                }
            ).first->second;

            itcLock = itcLockSP.Pn();
        } catch (const GpException& ex)
        {
            GpStringUtils::SCerr(ex.what());
        } catch (const std::exception& ex)
        {
            GpStringUtils::SCerr(ex.what());
        } catch (...)
        {
            GpStringUtils::SCerr("Unknown exception while Commit");
        }
    }

    return itcLock;
}

template<typename KeyT>
template<typename K>
bool    GpItcKeyBasedLock<KeyT>::ShardOfLocks::Release (K&& aKey) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    auto iter = iLocksMap.find(KeyT{aKey});
    auto&[itcLockSP, acquireCounter] = iter->second;

    if (acquireCounter > 1)
    {
        acquireCounter--;
    } else
    {
        try
        {
            // Return itcLock to pool
            iItcLocksPool.Release(std::move(itcLockSP));

            // Remove itcLock from iLocksMap
            iLocksMap.erase(iter);
        } catch (const GpException& ex)
        {
            GpStringUtils::SCerr(ex.what());
            return false;
        } catch (const std::exception& ex)
        {
            GpStringUtils::SCerr(ex.what());
            return false;
        } catch (...)
        {
            GpStringUtils::SCerr("Unknown exception while Commit");
            return false;
        }
    }

    return true;
}

// ---------------- Shards ----------------

template<typename KeyT>
void    GpItcKeyBasedLock<KeyT>::Shards::Init
(
    const size_t aShardsCount,
    const size_t aItcLocksPoolInitSize
)
{
    iShards.resize(aShardsCount);

    for (std::unique_ptr<ShardOfLocks>& shardPtr: iShards)
    {
        shardPtr = std::make_unique<ShardOfLocks>();
        shardPtr->Init(aItcLocksPoolInitSize);
    }
}

template<typename KeyT>
template<typename K>
GpItcLock*  GpItcKeyBasedLock<KeyT>::Shards::Acquire (K&& aKey) noexcept
{
    const size_t    shardIndex      = std::hash<KeyT>{}(KeyT{aKey}) % std::size(iShards);
    ShardOfLocks&   shardOfLocks    = *iShards[shardIndex].get();

    return shardOfLocks.Acquire(std::forward<K>(aKey));
}

template<typename KeyT>
template<typename K>
bool    GpItcKeyBasedLock<KeyT>::Shards::Release (K&& aKey) noexcept
{
    const size_t    shardIndex      = std::hash<KeyT>{}(KeyT{aKey}) % std::size(iShards);
    ShardOfLocks&   shardOfLocks    = *iShards[shardIndex].get();

    return shardOfLocks.Release(aKey);
}

// ---------------- GpItcKeyBasedLock ----------------
template<typename KeyT>
GpItcKeyBasedLock<KeyT>::GpItcKeyBasedLock
(
    const size_t aShardsCount,
    const size_t aItcLocksPoolInitSize
)
{
    Init(aShardsCount, aItcLocksPoolInitSize);
}

template<typename KeyT>
void    GpItcKeyBasedLock<KeyT>::Init
(
    const size_t aShardsCount,
    const size_t aItcLocksPoolInitSize
)
{
    iShards.Init(aShardsCount, aItcLocksPoolInitSize);
}

template<typename KeyT>
template<typename K>
GpItcLock*  GpItcKeyBasedLock<KeyT>::AcquireLock (K&& aKey) noexcept
{
    return iShards.Acquire(aKey);
}

template<typename KeyT>
template<typename K>
bool    GpItcKeyBasedLock<KeyT>::ReleaseLock (K&& aKey) noexcept
{
    return iShards.Release(aKey);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
