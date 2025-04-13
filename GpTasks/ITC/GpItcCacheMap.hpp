#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>
#include <GpCore2/GpTasks/ITC/GpItcLockRW.hpp>
#include <GpCore2/GpTasks/ITC/GpItcKeyBasedLock.hpp>

namespace GPlatform {

TAG_REGISTER(GpItcCacheMap)

template<typename ContainerT>
class GpItcCacheMap
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcCacheMap)
    TAG_SET(THREAD_SAFE)
    TAG_SET(GpItcCacheMap)

    using this_type                 = GpItcCacheMap<ContainerT>;
    using container_type            = ContainerT;
    using key_type                  = typename ContainerT::key_type;
    using mapped_type               = typename ContainerT::mapped_type;
    using KeyBasedLocksT            = GpItcKeyBasedLock<key_type>;

    template<typename T>
    using UpdateResT                = std::tuple<T, GpContainerUpdateStatus>;
    using GenerateFnT               = std::function<mapped_type()>;
    using ValidateFnT               = std::function<bool(const mapped_type& aValue)>;
    using IsUseGeneratedValueFnT    = std::function<bool(const mapped_type& aInCacheValue, const mapped_type& aGeneratedValue)>;
    using EvictionFnT               = std::function<void(ContainerT&)>;
    template<typename R>
    using TransformFnT              = std::function<R(mapped_type& aValue)>;

    enum class KeyLockMode
    {
        USE_KEY_LOCK,
        KEY_ALREADY_LOCKED
    };

    class UniqueKeyLock
    {
        CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(UniqueKeyLock)

    public:
        UniqueKeyLock   (key_type&&         aKey,
                         KeyLockMode        aKeyLockMode,
                         KeyBasedLocksT&    aKeyBasedLocks): iKey{std::move(aKey)}, iKeyBasedLocks{aKeyBasedLocks}

        {
            if (aKeyLockMode == KeyLockMode::USE_KEY_LOCK)
            {
                iItcLock = iKeyBasedLocks.AcquireLock(iKey);

                VERIFY
                (
                    iItcLock != nullptr,
                    "Failed to acquire lock"
                );

                iItcLock->lock();
            }
        }

        ~UniqueKeyLock  (void) noexcept
        {
            if (iItcLock)
            {
                iItcLock->unlock();
                std::ignore = iKeyBasedLocks.ReleaseLock(iKey);
            }
        }

    private:
        const key_type  iKey;
        KeyBasedLocksT& iKeyBasedLocks;
        GpItcLock*      iItcLock = nullptr;
    };

public:
    explicit                    GpItcCacheMap       (size_t aMaxSize,
                                                     size_t aKeyLocksShardsCount,
                                                     size_t aKeyLocksPoolInitSize) noexcept;
    virtual                     ~GpItcCacheMap      (void) noexcept;

    size_t                      Size                (void) const noexcept;
    size_t                      MaxSize             (void) const noexcept;
    bool                        Empty               (void) const noexcept;
    void                        Clear               (void) noexcept;

    template<typename K>
    GpItcLock*                  AcquireLockForKey   (K&& aKey) noexcept;

    template<typename K>
    [[nodiscard]]  bool         ReleaseLockForKey   (K&& aKey) noexcept;

    template<typename K>
    bool                        Erase               (K&&            aKey,
                                                     KeyLockMode    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

    template<typename K>
    mapped_type                 Find                (K&& aKey);

    template<typename R,
             typename K>
    R                           Find                (K&&                    aKey,
                                                     const TransformFnT<R>& aTransform);

    template<typename K>
    std::optional<mapped_type>  FindOpt             (K&& aKey);

    template<typename R,
             typename K>
    std::optional<R>            FindOpt             (K&&                    aKey,
                                                     const TransformFnT<R>& aTransform);

    template<typename K>
    UpdateResT<mapped_type>     FindOrGenerate      (K&&                            aKey,
                                                     const GenerateFnT&             aGenerateFn,
                                                     const ValidateFnT&             aValidateFn,
                                                     const IsUseGeneratedValueFnT&  aIsUseGeneratedValueFn,
                                                     const EvictionFnT&             aEvictionFn,
                                                     KeyLockMode                    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

    template<typename R,
             typename K>
    UpdateResT<R>               FindOrGenerate      (K&&                            aKey,
                                                     const GenerateFnT&             aGenerateFn,
                                                     const ValidateFnT&             aValidateFn,
                                                     const IsUseGeneratedValueFnT&  aIsUseGeneratedValueFn,
                                                     const EvictionFnT&             aEvictionFn,
                                                     const TransformFnT<R>&         aTransform,
                                                     KeyLockMode                    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

    template<typename K,
             typename M>
    UpdateResT<mapped_type>     FindOrSet           (K&&                            aKey,
                                                     M&&                            aValue,
                                                     const ValidateFnT&             aValidateFn,
                                                     const EvictionFnT&             aEvictionFn,
                                                     KeyLockMode                    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

    template<typename R,
             typename K,
             typename M>
    UpdateResT<R>               FindOrSet           (K&&                            aKey,
                                                     M&&                            aValue,
                                                     const ValidateFnT&             aValidateFn,
                                                     const EvictionFnT&             aEvictionFn,
                                                     const TransformFnT<R>&         aTransform,
                                                     KeyLockMode                    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

    template<typename K,
             typename M>
    mapped_type                 Set                 (K&&                            aKey,
                                                     M&&                            aValue,
                                                     const EvictionFnT&             aEvictionFn,
                                                     KeyLockMode                    aKeyLockMode = KeyLockMode::USE_KEY_LOCK);

private:
    mutable GpItcLockRW     iItcLockRW;
    ContainerT              iContainer      GUARDED_BY(iItcLockRW);
    KeyBasedLocksT          iKeyBasedLocks;
    const size_t            iMaxSize        GUARDED_BY(iItcLockRW) = 0;
};

template<typename ContainerT>
GpItcCacheMap<ContainerT>::GpItcCacheMap
(
    const size_t aMaxSize,
    const size_t aKeyLocksShardsCount,
    const size_t aKeyLocksPoolInitSize
) noexcept:
iKeyBasedLocks{aKeyLocksShardsCount, aKeyLocksPoolInitSize},
iMaxSize      {aMaxSize}
{
}

template<typename ContainerT>
GpItcCacheMap<ContainerT>::~GpItcCacheMap (void) noexcept
{
    Clear();
}

template<typename ContainerT>
size_t  GpItcCacheMap<ContainerT>::Size (void) const noexcept
{
    GpSharedLock<GpItcLockRW> sharedLock{iItcLockRW};

    return std::size(iContainer);
}

template<typename ContainerT>
size_t  GpItcCacheMap<ContainerT>::MaxSize (void) const noexcept
{
    return iMaxSize;
}

template<typename ContainerT>
bool    GpItcCacheMap<ContainerT>::Empty (void) const noexcept
{
    GpSharedLock<GpItcLockRW> sharedLock{iItcLockRW};

    return iContainer.empty();
}

template<typename ContainerT>
void    GpItcCacheMap<ContainerT>::Clear (void) noexcept
{
    GpUniqueLock<GpItcLockRW> uniqueLock{iItcLockRW};

    return iContainer.clear();
}

template<typename ContainerT>
template<typename K>
GpItcLock*  GpItcCacheMap<ContainerT>::AcquireLockForKey (K&& aKey) noexcept
{
    return iKeyBasedLocks.AcquireLock(aKey);
}

template<typename ContainerT>
template<typename K>
bool    GpItcCacheMap<ContainerT>::ReleaseLockForKey (K&& aKey) noexcept
{
    return iKeyBasedLocks.ReleaseLock(aKey);
}

template<typename ContainerT>
template<typename K>
bool    GpItcCacheMap<ContainerT>::Erase
(
    K&&         aKey,
    KeyLockMode aKeyLockMode
)
{
    UniqueKeyLock               uniqueLockForKey{key_type{aKey}, aKeyLockMode, iKeyBasedLocks};
    GpUniqueLock<GpItcLockRW>   uniqueLock{iItcLockRW};

    return iContainer.erase(aKey) > 0;
}

template<typename ContainerT>
template<typename K>
auto    GpItcCacheMap<ContainerT>::Find (K&& aKey) -> mapped_type
{
    static const TransformFnT<mapped_type> sTransfotmFn = [](mapped_type& aValue)
    {
        return mapped_type{aValue};
    };

    return Find
    (
        std::forward<K>(aKey),
        sTransfotmFn
    );
}

template<typename ContainerT>
template<typename R,
         typename K>
R   GpItcCacheMap<ContainerT>::Find
(
    K&&                     aKey,
    const TransformFnT<R>&  aTransform
)
{
    GpSharedLock<GpItcLockRW> sharedLock{iItcLockRW};

    auto iter = iContainer.find(aKey);

    VERIFY
    (
        iter != std::end(iContainer),
        [&]()
        {
            return fmt::format
            (
                "Element not found for key '{}'",
                std::to_string(aKey)
            );
        }
    );

    return aTransform(iter->second);
}

template<typename ContainerT>
template<typename K>
auto    GpItcCacheMap<ContainerT>::FindOpt (K&& aKey) -> std::optional<mapped_type>
{
    static const TransformFnT<mapped_type> sTransfotmFn = [](mapped_type& aValue)
    {
        return mapped_type{aValue};
    };

    return FindOpt
    (
        std::forward<K>(aKey),
        sTransfotmFn
    );
}

template<typename ContainerT>
template<typename R,
         typename K>
std::optional<R>    GpItcCacheMap<ContainerT>::FindOpt
(
    K&&                     aKey,
    const TransformFnT<R>&  aTransform
)
{
    GpSharedLock<GpItcLockRW> sharedLock{iItcLockRW};

    auto iter = iContainer.find(aKey);

    if (iter != std::end(iContainer))
    {
        return aTransform(iter->second);
    } else
    {
        return std::nullopt;
    }
}

template<typename ContainerT>
template<typename K>
auto    GpItcCacheMap<ContainerT>::FindOrGenerate
(
    K&&                             aKey,
    const GenerateFnT&              aGenerateFn,
    const ValidateFnT&              aValidateFn,
    const IsUseGeneratedValueFnT&   aIsUseGeneratedValueFn,
    const EvictionFnT&              aEvictionFn,
    KeyLockMode                     aKeyLockMode
) -> UpdateResT<mapped_type>
{
    static const TransformFnT<mapped_type> sTransfotmFn = [](mapped_type& aValue)
    {
        return mapped_type{aValue};
    };

    return FindOrGenerate<mapped_type>
    (
        std::forward<K>(aKey),
        aGenerateFn,
        aValidateFn,
        aIsUseGeneratedValueFn,
        aEvictionFn,
        sTransfotmFn,
        aKeyLockMode
    );
}

template<typename ContainerT>
template<typename R,
         typename K>
auto    GpItcCacheMap<ContainerT>::FindOrGenerate
(
    K&&                             aKey,
    const GenerateFnT&              aGenerateFn,
    const ValidateFnT&              aValidateFn,
    const IsUseGeneratedValueFnT&   aIsUseGeneratedValueFn,
    const EvictionFnT&              aEvictionFn,
    const TransformFnT<R>&          aTransform,
    KeyLockMode                     aKeyLockMode
) -> UpdateResT<R>
{
    // Search cache with shared lock
    {
        GpSharedLock<GpItcLockRW> sharedLock{iItcLockRW};

        // Search the cache
        auto iter = iContainer.find(aKey);

        // Validate if found
        if (iter != std::end(iContainer)) [[likely]]
        {
            // Validate (non-blocking)
            if (aValidateFn(iter->second)) [[likely]]
            {
                return
                {
                    aTransform(iter->second),
                    GpContainerUpdateStatus::KEEP_EXISTING
                };
            }
        }
    }

    // Value not found in cache. Generate new one
    mapped_type generatedValue = aGenerateFn();

    // Insert into cache (unique lock)
    {
        UniqueKeyLock               uniqueLockForKey{key_type{aKey}, aKeyLockMode, iKeyBasedLocks};
        GpUniqueLock<GpItcLockRW>   uniqueLock{iItcLockRW};

        // Search in cache
        auto iter = iContainer.find(aKey);

        // Check if found
        if (iter != std::end(iContainer))
        {
            mapped_type& valueFromCache = iter->second;

            // Select value (cache or generated)
            if (aValidateFn(valueFromCache) && !aIsUseGeneratedValueFn(valueFromCache, generatedValue)) [[unlikely]]
            {
                // Discard generated value, use cache value
                return
                {
                    aTransform(valueFromCache),
                    GpContainerUpdateStatus::KEEP_EXISTING
                };
            } else
            {
                // Update value to generated
                iter->second = std::move(generatedValue);

                // Return
                return
                {
                    aTransform(iter->second),
                    GpContainerUpdateStatus::INSERT_NEW
                };
            }
        } else
        {
            // Check cache size
            if (std::size(iContainer) >= iMaxSize) [[unlikely]]
            {
                aEvictionFn(iContainer);
            }

            // Insert generated value into cache (no hint)
            mapped_type& val = iContainer.insert_or_assign
            (
                key_type{std::forward<K>(aKey)},
                std::move(generatedValue)
            ).first->second;

            // Return
            return
            {
                aTransform(val),
                GpContainerUpdateStatus::INSERT_NEW
            };
        }
    }
}

template<typename ContainerT>
template<typename K,
         typename M>
auto    GpItcCacheMap<ContainerT>::FindOrSet
(
    K&&                 aKey,
    M&&                 aValue,
    const ValidateFnT&  aValidateFn,
    const EvictionFnT&  aEvictionFn,
    const KeyLockMode   aKeyLockMode
) -> UpdateResT<mapped_type>
{
    const GenerateFnT generateFn = [&]() -> mapped_type
    {
        return mapped_type{std::forward<M>(aValue)};
    };

    static const IsUseGeneratedValueFnT sIsUseGeneratedValueFn = [](const mapped_type&, const mapped_type&) -> bool
    {
        return true;
    };

    return FindOrGenerate
    (
        std::forward<K>(aKey),
        generateFn,
        aValidateFn,
        sIsUseGeneratedValueFn,
        aEvictionFn,
        aKeyLockMode
    );
}

template<typename ContainerT>
template<typename R,
         typename K,
         typename M>
auto    GpItcCacheMap<ContainerT>::FindOrSet
(
    K&&                     aKey,
    M&&                     aValue,
    const ValidateFnT&      aValidateFn,
    const EvictionFnT&      aEvictionFn,
    const TransformFnT<R>&  aTransform,
    const KeyLockMode       aKeyLockMode
) -> UpdateResT<R>
{
    const GenerateFnT generateFn = [&]() -> mapped_type
    {
        return mapped_type{std::forward<M>(aValue)};
    };

    static const IsUseGeneratedValueFnT sIsUseGeneratedValueFn = [](const mapped_type&, const mapped_type&) -> bool
    {
        return true;
    };

    return FindOrGenerate
    (
        std::forward<K>(aKey),
        generateFn,
        aValidateFn,
        sIsUseGeneratedValueFn,
        aEvictionFn,
        aTransform,
        aKeyLockMode
    );
}

template<typename ContainerT>
template<typename K,
         typename M>
auto    GpItcCacheMap<ContainerT>::Set
(
    K&&                 aKey,
    M&&                 aValue,
    const EvictionFnT&  aEvictionFn,
    const KeyLockMode   aKeyLockMode
) -> mapped_type
{
    static const ValidateFnT sValidateFn = [](const mapped_type&) -> bool
    {
        return false;
    };

    return std::get<0>
    (
        FindOrSet
        (
            std::forward<K>(aKey),
            std::forward<M>(aValue),
            sValidateFn,
            aEvictionFn,
            aKeyLockMode
        )
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
