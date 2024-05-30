// GTags:
// #DOC_ADDED     (2024-05-23)
// #DOC_VALIDATED (2024-05-23)

#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

#include <functional>

namespace GPlatform {

TAG_REGISTER(GpCacheMap)

/**
 * @brief A thread-safe cache map.
 *
 * This class provides a thread-safe cache map with customizable value generation and validation functions.
 *
 * @tparam ContainerT The type of the underlying container.
 */
template<typename ContainerT>
class GpCacheMap
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpCacheMap)
    TAG_SET(THREAD_SAFE)
    TAG_SET(GpCacheMap)

    using this_type         = GpCacheMap<ContainerT>;
    using container_type    = ContainerT;
    using key_type          = typename ContainerT::key_type;
    using mapped_type       = typename ContainerT::mapped_type;

    template<typename T>
    using ValueOnGetFnT     = std::function<T(mapped_type&)>;
    using ValueGenerateFnT  = std::function<mapped_type()>;
    using ValueIsValidFnT   = std::function<bool(const mapped_type& aValue)>;
    using RemoveOneOldFnT   = std::function<void(ContainerT&)>;

public:
    /**
     * @brief Constructor.
     * @param aMaxSize The maximum size of the cache.
     */
                            GpCacheMap          (const size_t aMaxSize) noexcept;

    /**
     * @brief Destructor.
     */
                            ~GpCacheMap         (void) noexcept = default;

    /**
     * @brief Get the current size of the cache.
     * @return The number of elements in the cache.
     */
    size_t                  Size                (void) const noexcept;

    /**
     * @brief Get or generate a new value.
     *
     * If the value associated with the given key is present in the cache and valid, it is returned.
     * Otherwise, a new value is generated, inserted into the cache, and returned.
     *
     * @tparam K The type of the key.
     * @param aKey The key to lookup in the cache.
     * @param aGenerateFn The function to generate a new value if the key is not present (will call without any lock).
     * @param aValidateFn The function to validate the cached value.
     * @param aRemoveOneOldFn The function to remove one old value if the cache is full.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type             GetOrGenerateNew    (const K&           aKey,
                                                 ValueGenerateFnT   aGenerateFn,
                                                 ValueIsValidFnT    aValidateFn,
                                                 RemoveOneOldFnT    aRemoveOneOldFn);

    /**
     * @brief Get or generate a new value and apply a function on it.
     *
     * If the value associated with the given key is present in the cache and valid, the function is applied on it and the result is returned.
     * Otherwise, a new value is generated, inserted into the cache, the function is applied on it, and the result is returned.
     *
     * @tparam K The type of the key.
     * @tparam R The return type of the function applied on the value.
     * @param aKey The key to lookup in the cache.
     * @param aOnGetFn The function to apply on the value.
     * @param aGenerateFn The function to generate a new value if the key is not present (will call without any lock).
     * @param aValidateFn The function to validate the cached value.
     * @param aRemoveOneOldFn The function to remove one old value if the cache is full.
     * @return The result of the function applied on the value.
     */
    template<typename K,
             typename R>
    R                       GetOrGenerateNew    (const K&           aKey,
                                                 ValueOnGetFnT<R>   aOnGetFn,
                                                 ValueGenerateFnT   aGenerateFn,
                                                 ValueIsValidFnT    aValidateFn,
                                                 RemoveOneOldFnT    aRemoveOneOldFn);

private:
    mutable GpSpinLockRW    iSpinLockRW;
    ContainerT              iContainer  GUARDED_BY(iSpinLockRW);
    const size_t            iMaxSize    GUARDED_BY(iSpinLockRW) = 0;
};

template<typename ContainerT>
GpCacheMap<ContainerT>::GpCacheMap (const size_t aMaxSize) noexcept:
iMaxSize{aMaxSize}
{
}

template<typename ContainerT>
size_t  GpCacheMap<ContainerT>::Size (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return std::size(iContainer);
}

template<typename ContainerT>
template<typename K>
GpCacheMap<ContainerT>::mapped_type GpCacheMap<ContainerT>::GetOrGenerateNew
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn,
    ValueIsValidFnT     aValidateFn,
    RemoveOneOldFnT     aRemoveOneOldFn
)
{
    // Try to find in cache (shared lock)
    {
        GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (   (iter != std::end(iContainer))
            && (aValidateFn(iter->second)))
        {
            // Found in cache, return value
            return iter->second;
        }
    }

    // Gen value
    auto generatedValue = aGenerateFn();

    // Try to place into cache (unique lock)
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

        if (std::size(iContainer) >= iMaxSize)
        {
            aRemoveOneOldFn(iContainer);
        }

        mapped_type& val = iContainer.insert_or_assign
        (
            key_type{aKey},
            std::move(generatedValue)
        ).first->second;

        return val;
    }
}

template<typename ContainerT>
template<typename K,
         typename R>
R   GpCacheMap<ContainerT>::GetOrGenerateNew
(
    const K&            aKey,
    ValueOnGetFnT<R>    aOnGetFn,
    ValueGenerateFnT    aGenerateFn,
    ValueIsValidFnT     aValidateFn,
    RemoveOneOldFnT     aRemoveOneOldFn
)
{
    // Try to find in cache (shared lock)
    {
        GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (   (iter != std::end(iContainer))
            && (aValidateFn(iter->second)))
        {
            // Found in cache, return value
            return aOnGetFn(iter->second);
        }
    }

    // Gen value
    auto generatedValue = aGenerateFn();

    // Try to place into cache (unique lock)
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

        if (std::size(iContainer) >= iMaxSize)
        {
            aRemoveOneOldFn(iContainer);
        }

        mapped_type& val = iContainer.insert_or_assign
        (
            key_type{aKey},
            std::move(generatedValue)
        ).first->second;

        return aOnGetFn(val);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
