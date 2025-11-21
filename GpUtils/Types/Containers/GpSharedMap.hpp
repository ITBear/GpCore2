#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainerUpdateStatus.hpp>

#include <functional>
#include <optional>

namespace GPlatform {

TAG_REGISTER(GpSharedMap)

template<typename ContainerT>
class GpSharedMap
{
    CLASS_REMOVE_CTRS_COPY(GpSharedMap)
    TAG_SET(THREAD_SAFE)
    TAG_SET(GpSharedMap)

public:
    using this_type         = GpSharedMap<ContainerT>;
    using container_type    = ContainerT;
    using key_type          = typename ContainerT::key_type;
    using mapped_type       = typename ContainerT::mapped_type;

    using UpdateResT        = std::tuple<mapped_type, GpContainerUpdateStatus>;
    using GenerateFnT       = std::function<mapped_type()>;
    using ProcessTnT        = std::function<void(ContainerT&)>;
    using ApplyFnT          = std::function<void(mapped_type&)>;

public:
                                GpSharedMap     (void) noexcept;
                                GpSharedMap     (GpSharedMap&& aMap) noexcept;
                                ~GpSharedMap    (void) noexcept;

    size_t                      Size            (void) const noexcept;
    bool                        Empty           (void) const noexcept;
    void                        Clear           (void) noexcept;

    template<typename K>
    mapped_type                 Find            (K&& aKey) const;

    template<typename K>
    std::optional<mapped_type>  FindOpt         (K&& aKey) const;

    template<typename K>
    mapped_type                 FindOrDefault   (K&& aKey, mapped_type aDefaultValue) const;

    template<typename K>
    UpdateResT                  FindOrGenerate  (K&&                    aKey,
                                                 const GenerateFnT&     aGenerateFn);
    template<typename K,
             typename M>
    UpdateResT                  FindOrSet       (K&& aKey,
                                                 M&& aValue);

    template<typename K,
             typename M>
    void                        Set             (K&& aKey,
                                                 M&& aValue);

    template<typename K>
    bool                        Erase           (K&& aKey);

    template<typename K>
    std::optional<mapped_type>  Extract         (K&& aKey);
    this_type                   ExtractAll      (void) noexcept;

    void                        Process         (const ProcessTnT& aProcessFn);
    void                        Apply           (const ApplyFnT& aApplyFn);

private:
    mutable GpSpinLockRW<>  iSpinLockRW;
    ContainerT              iContainer GUARDED_BY(iSpinLockRW);
};

template<typename ContainerT>
GpSharedMap<ContainerT>::GpSharedMap (void) noexcept
{
}

template<typename ContainerT>
GpSharedMap<ContainerT>::GpSharedMap (GpSharedMap&& aMap) noexcept:
iContainer{std::move(aMap.iContainer)}
{
}

template<typename ContainerT>
GpSharedMap<ContainerT>::~GpSharedMap (void) noexcept
{
    Clear();
}

template<typename ContainerT>
size_t  GpSharedMap<ContainerT>::Size (void) const noexcept
{
    GpSharedLock sharedLock{iSpinLockRW};

    return std::size(iContainer);
}

template<typename ContainerT>
bool    GpSharedMap<ContainerT>::Empty (void) const noexcept
{
    GpSharedLock sharedLock{iSpinLockRW};

    return iContainer.empty();
}

template<typename ContainerT>
void    GpSharedMap<ContainerT>::Clear (void) noexcept
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    return iContainer.clear();
}

template<typename ContainerT>
template<typename K>
auto    GpSharedMap<ContainerT>::Find (K&& aKey) const -> mapped_type
{
    GpSharedLock sharedLock{iSpinLockRW};

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

    return iter->second;
}

template<typename ContainerT>
template<typename K>
auto    GpSharedMap<ContainerT>::FindOpt (K&& aKey) const -> std::optional<mapped_type>
{
    GpSharedLock sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    if (iter != std::end(iContainer))
    {
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename ContainerT>
template<typename K>
auto    GpSharedMap<ContainerT>::FindOrDefault (K&& aKey, mapped_type aDefaultValue) const -> mapped_type
{
    GpSharedLock sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    if (iter != std::end(iContainer))
    {
        return iter->second;
    } else
    {
        return aDefaultValue;
    }
}

template<typename ContainerT>
template<typename K>
auto    GpSharedMap<ContainerT>::FindOrGenerate
(
    K&&                 aKey,
    const GenerateFnT&  aGenerateFn
) -> UpdateResT
{
    // Try to find (shared lock)
    {
        GpSharedLock sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return UpdateResT
            {
                iter->second,
                GpContainerUpdateStatus::KEEP_EXISTING
            };
        }
    }

    // Generate new value (without lock)
    mapped_type newValue = aGenerateFn();

    // Try to find and insert new value (unique lock)
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        // Try to find
        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return UpdateResT
            {
                iter->second,
                GpContainerUpdateStatus::KEEP_EXISTING
            };
        }

        // Insert new value
        return UpdateResT
        {
            iContainer.emplace
            (
                key_type{std::forward<K>(aKey)},
                std::move(newValue)
            ).first->second,
            GpContainerUpdateStatus::INSERT_NEW
        };
    }
}

template<typename ContainerT>
template<typename K,
         typename M>
auto    GpSharedMap<ContainerT>::FindOrSet
(
    K&& aKey,
    M&& aValue
) -> UpdateResT
{
    // Try to find (shared lock)
    {
        GpSharedLock sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return UpdateResT
            {
                iter->second,
                GpContainerUpdateStatus::KEEP_EXISTING
            };
        }
    }

    // Try to find and insert new value (unique lock)
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        // Try to find
        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return UpdateResT
            {
                iter->second,
                GpContainerUpdateStatus::KEEP_EXISTING
            };
        }

        // Insert new value
        return UpdateResT
        {
            iContainer.emplace
            (
                key_type{std::forward<K>(aKey)},
                mapped_type{std::forward<M>(aValue)}
            ).first->second,
            GpContainerUpdateStatus::INSERT_NEW
        };
    }
}

template<typename ContainerT>
template<typename K,
         typename M>
void    GpSharedMap<ContainerT>::Set
(
    K&& aKey,
    M&& aValue
)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    iContainer.insert_or_assign
    (
        key_type{std::forward<K>(aKey)},
        mapped_type{std::forward<M>(aValue)}
    );
}

template<typename ContainerT>
template<typename K>
bool    GpSharedMap<ContainerT>::Erase (K&& aKey)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    return iContainer.erase(aKey) > 0;
}

template<typename ContainerT>
template<typename K>
auto    GpSharedMap<ContainerT>::Extract (K&& aKey) -> std::optional<mapped_type>
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    // Try to find
    auto iter = iContainer.find(aKey);
    if (iter == std::end(iContainer))
    {
        return std::nullopt;
    }

    // Get value
    mapped_type value = std::move(iter->second);

    // Erase
    iContainer.erase(iter);

    //
    return value;
}

template<typename ContainerT>
auto    GpSharedMap<ContainerT>::GpSharedMap<ContainerT>::ExtractAll (void) noexcept -> this_type
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    return this_type{std::move(*this)};
}

template<typename ContainerT>
void    GpSharedMap<ContainerT>::Process (const ProcessTnT& aProcessFn)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    aProcessFn(iContainer);
}

template<typename ContainerT>
void    GpSharedMap<ContainerT>::Apply (const ApplyFnT& aApplyFn)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    for (auto& e: iContainer)
    {
        aApplyFn(e.second);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
