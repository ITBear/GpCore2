// GTags:
// #DOC_ADDED     (2024-05-24)
// #DOC_VALIDATED (2024-05-24)

#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

#include <functional>
#include <optional>

namespace GPlatform {

TAG_REGISTER(GpDictionary)

/**
 * @brief A thread-safe dictionary class.
 *
 * This class provides a thread-safe dictionary with various methods for accessing and modifying the data.
 *
 * @tparam ContainerT The type of the underlying container.
 */
template<typename ContainerT>
class GpDictionary
{
    CLASS_REMOVE_CTRS_COPY(GpDictionary)
    TAG_SET(THREAD_SAFE)
    TAG_SET(GpDictionary)

public:
    using this_type         = GpDictionary<ContainerT>;
    using container_type    = ContainerT;
    using key_type          = typename ContainerT::key_type;
    using mapped_type       = typename ContainerT::mapped_type;

    template<typename T>
    using ValueOnGetFnT     = std::function<T(mapped_type&)>;
    using ValueGenerateFnT  = std::function<mapped_type()>;

public:
    /**
     * @brief Default constructor.
     */
                                GpDictionary        (void) noexcept;

    /**
     * @brief Move constructor.
     * @param aCatalog The dictionary to move.
     */
                                GpDictionary        (GpDictionary&& aCatalog) noexcept;
    /**
     * @brief Destructor.
     */
                                ~GpDictionary       (void) noexcept;

    /**
     * @brief Get the size of the dictionary.
     * @return The number of elements in the dictionary.
     */
    size_t                      Size                (void) const noexcept;

    /**
     * @brief Check if the dictionary is empty.
     * @return True if the dictionary is empty, otherwise false.
     */
    bool                        Empty               (void) const noexcept;

    /**
     * @brief Clear the dictionary.
     */
    void                        Clear               (void) noexcept;

    /**
     * @brief Get the value associated with the given key. Throws exception if not found.
     * @tparam K The type of the key.
     * @param aKey The key to look up.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type                 Get                 (const K&           aKey) const;

    /**
     * @brief Get the value associated with the given key and apply a function to it. Throws exception if not found.
     * @tparam K The type of the key.
     * @tparam R The return type of the function.
     * @param aKey The key to look up.
     * @param aOnGetFn The function to apply to the value.
     * @return The result of the function applied to the value.
     */
    template<typename K,
             typename R>
    R                           Get                 (const K&           aKey,
                                                     ValueOnGetFnT<R>   aOnGetFn) const;

    /**
     * @brief Get the value associated with the given key if it exists.
     * @tparam K The type of the key.
     * @param aKey The key to look up.
     * @return An optional containing the value if it exists, otherwise std::nullopt.
     */
    template<typename K>
    std::optional<mapped_type>  GetOpt              (const K&           aKey) const;

    /**
     * @brief Get the value associated with the given key and apply a function to it if it exists.
     * @tparam K The type of the key.
     * @tparam R The return type of the function.
     * @param aKey The key to look up.
     * @param aOnGetFn The function to apply to the value.
     * @return An optional containing the result of the function if the value exists, otherwise std::nullopt.
     */
    template<typename K,
             typename R>
    std::optional<R>            GetOpt              (const K&           aKey,
                                                     ValueOnGetFnT<R>   aOnGetFn) const;

    /**
     * @brief Get the value associated with the given key or generate a new value if it does not exist.
     * @tparam K The type of the key.
     * @param aKey The key to look up.
     * @param aGenerateFn The function to generate a new value if the key does not exist.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type                 GetOrGenerateNew    (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn);

    /**
     * @brief Get the value associated with the given key or generate a new value if it does not exist, and apply a function to it.
     * @tparam K The type of the key.
     * @tparam R The return type of the function.
     * @param aKey The key to look up.
     * @param aGenerateFn The function to generate a new value if the key does not exist.
     * @param aOnGetFn The function to apply to the value.
     * @return The result of the function applied to the value.
     */
    template<typename K,
             typename R>
    R                           GetOrGenerateNew    (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn,
                                                     ValueOnGetFnT<R>   aOnGetFn);

    /**
     * @brief Set or update the value associated with the given key.
     * @tparam K The type of the key.
     * @param aKey The key to set or update.
     * @param aValue The value to set or update.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type                 SetOrUpdate         (const K&           aKey,
                                                     const mapped_type& aValue);

    /**
     * @brief Set or update the value associated with the given key.
     * @tparam K The type of the key.
     * @param aKey The key to set or update.
     * @param aValue The value to set or update.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type                 SetOrUpdate         (const K&           aKey,
                                                     mapped_type&&      aValue);

    /**
     * @brief Set or update the value associated with the given key using a generate function.
     * @tparam K The type of the key.
     * @param aKey The key to set or update.
     * @param aGenerateFn The function to generate a new value.
     * @return The value associated with the key.
     */
    template<typename K>
    mapped_type                 SetOrUpdate         (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn);

    /**
     * @brief Set or update the value associated with the given key using a generate function and apply a function to it.
     * @tparam K The type of the key.
     * @tparam R The return type of the function.
     * @param aKey The key to set or update.
     * @param aGenerateFn The function to generate a new value.
     * @param aOnGetFn The function to apply to the value.
     * @return The result of the function applied to the value.
     */
    template<typename K,
             typename R>
    R                           SetOrUpdate         (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn,
                                                     ValueOnGetFnT<R>   aOnGetFn);

    /**
     * @brief Try to set the value associated with the given key if it does not exist.
     * @tparam K The type of the key.
     * @param aKey The key to set.
     * @param aValue The value to set.
     * @return A tuple containing the value associated with the key and a boolean indicating whether the value was newly inserted.
     */
    template<typename K>
    std::tuple<mapped_type, bool>TrySet             (const K&           aKey,
                                                     const mapped_type& aValue);

    /**
     * @brief Try to set the value associated with the given key if it does not exist.
     * @tparam K The type of the key.
     * @param aKey The key to set.
     * @param aValue The value to set.
     * @return A tuple containing the value associated with the key and a boolean indicating whether the value was newly inserted.
     */
    template<typename K>
    std::tuple<mapped_type, bool>TrySet             (const K&           aKey,
                                                     mapped_type&&      aValue);

    /**
     * @brief Try to set the value associated with the given key using a generate function if it does not exist.
     * @tparam K The type of the key.
     * @param aKey The key to set.
     * @param aGenerateFn The function to generate a new value.
     * @return A tuple containing the value associated with the key and a boolean indicating whether the value was newly inserted.
     */
    template<typename K>
    std::tuple<mapped_type, bool>TrySet             (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn);

    /**
     * @brief Try to set the value associated with the given key using a generator function and apply a function to it.
     * @tparam K The type of the key.
     * @tparam R The return type of the function.
     * @param aKey The key to set in the dictionary.
     * @param aGenerateFn The function to generate the value to associate with the key.
     * @param aOnGetFn The function to apply to the value.
     * @return A tuple containing the result of the function applied to the value and a boolean indicating whether the value was set.
     */
    template<typename K,
             typename R>
    std::tuple<R, bool>         TrySet              (const K&           aKey,
                                                     ValueGenerateFnT   aGenerateFn,
                                                     ValueOnGetFnT<R>   aOnGetFn);

    /**
     * @brief Erase the value associated with the given key. Throws exception if not found.
     * @tparam K The type of the key.
     * @param aKey The key to erase in the dictionary.
     * @return The value that was associated with the key.
     */
    template<typename K>
    mapped_type                 Erase               (const K& aKey);

    /**
     * @brief Erase the value associated with the given key if it exists.
     * @tparam K The type of the key.
     * @param aKey The key to erase in the dictionary.
     * @return An optional containing the value that was associated with the key if it exists, otherwise std::nullopt.
     */
    template<typename K>
    std::optional<mapped_type>  EraseOpt            (const K& aKey);

    /**
     * @brief Extract all elements from the dictionary.
     * @return A new dictionary containing all the elements.
     */
    this_type                   ExtractAll          (void) noexcept;

    /**
     * @brief Process the container with a given function.
     * @param aFn The function to process the container.
     */
    void                        ProcessContainer    (std::function<void(ContainerT&)> aFn);


    /**
     * @brief Apply a function to all elements in the dictionary.
     * @param aFn The function to apply to each element.
     */
    void                        ApplyToAll          (std::function<void(mapped_type&)> aFn);

private:
    mutable GpSpinLockRW        iSpinLockRW;
    ContainerT                  iContainer GUARDED_BY(iSpinLockRW);
};

template<typename ContainerT>
GpDictionary<ContainerT>::GpDictionary (void) noexcept
{
}

template<typename ContainerT>
GpDictionary<ContainerT>::GpDictionary (GpDictionary&& aCatalog) noexcept:
iContainer{std::move(aCatalog.iContainer)}
{
}

template<typename ContainerT>
GpDictionary<ContainerT>::~GpDictionary (void) noexcept
{
    Clear();
}

template<typename ContainerT>
size_t  GpDictionary<ContainerT>::Size (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return std::size(iContainer);
}

template<typename ContainerT>
bool    GpDictionary<ContainerT>::Empty (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return iContainer.empty();
}

template<typename ContainerT>
void    GpDictionary<ContainerT>::Clear (void) noexcept
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iContainer.clear();
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::Get (const K& aKey) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    THROW_COND_GP
    (
        iter != std::end(iContainer),
        [&]()
        {
            return fmt::format
            (
                "Element not found by key '{}'",
                std::to_string(aKey)
            );
        }
    );

    return iter->second;
}

template<typename ContainerT>
template<typename K,
         typename R>
R   GpDictionary<ContainerT>::Get
(
    const K&            aKey,
    ValueOnGetFnT<R>    aOnGetFn
) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    THROW_COND_GP
    (
        iter != std::end(iContainer),
        [&]()
        {
            return fmt::format
            (
                "Element not found by key '{}'",
                std::to_string(aKey)
            );
        }
    );

    return aOnGetFn(iter->second);
}

template<typename ContainerT>
template<typename K>
std::optional<typename GpDictionary<ContainerT>::mapped_type>   GpDictionary<ContainerT>::GetOpt (const K& aKey) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    if (iter == std::end(iContainer)) [[unlikely]]
    {
        return std::nullopt;
    }

    return iter->second;
}

template<typename ContainerT>
template<typename K,
         typename R>
std::optional<R>    GpDictionary<ContainerT>::GetOpt
(
    const K&            aKey,
    ValueOnGetFnT<R>    aOnGetFn
) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    if (iter == std::end(iContainer)) [[unlikely]]
    {
        return std::nullopt;
    }

    return aOnGetFn(iter->second);
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::GetOrGenerateNew
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn
)
{
    // Try to find (shared lock)
    {
        GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return iter->second;
        }
    }

    // Gen value
    auto generatedValue = aGenerateFn();

    // Try to place into cache (unique lock)
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

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
R   GpDictionary<ContainerT>::GetOrGenerateNew
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn,
    ValueOnGetFnT<R>    aOnGetFn
)
{
    // Try to find (shared lock)
    {
        GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

        auto iter = iContainer.find(aKey);
        if (iter != std::end(iContainer))
        {
            return aOnGetFn(iter->second);
        }
    }

    // Gen value
    auto generatedValue = aGenerateFn();

    // Try to place into cache (unique lock)
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

        mapped_type& val = iContainer.insert_or_assign
        (
            key_type{aKey},
            std::move(generatedValue)
        ).first->second;

        return aOnGetFn(val);
    }
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::SetOrUpdate
(
    const K&            aKey,
    const mapped_type&  aValue
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.insert_or_assign(key_type{aKey}, aValue);

    return iter->second;
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::SetOrUpdate
(
    const K&        aKey,
    mapped_type&&   aValue
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.insert_or_assign(key_type{aKey}, std::move(aValue));

    return iter->second;
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::SetOrUpdate
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.insert_or_assign(key_type{aKey}, aGenerateFn());

    return iter->second;
}

template<typename ContainerT>
template<typename K,
         typename R>
R   GpDictionary<ContainerT>::SetOrUpdate
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn,
    ValueOnGetFnT<R>    aOnGetFn
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.insert_or_assign(key_type{aKey}, aGenerateFn());

    return aOnGetFn(iter->second);
}

template<typename ContainerT>
template<typename K>
std::tuple<typename GpDictionary<ContainerT>::mapped_type, bool>    GpDictionary<ContainerT>::TrySet
(
    const K&            aKey,
    const mapped_type&  aValue
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.try_emplace(key_type{aKey}, aValue);

    return {iter->second, isPlacedNew};
}

template<typename ContainerT>
template<typename K>
std::tuple<typename GpDictionary<ContainerT>::mapped_type, bool>    GpDictionary<ContainerT>::TrySet
(
    const K&        aKey,
    mapped_type&&   aValue
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.try_emplace(key_type{aKey}, std::move(aValue));

    return {iter->second, isPlacedNew};
}

template<typename ContainerT>
template<typename K>
std::tuple<typename GpDictionary<ContainerT>::mapped_type, bool>    GpDictionary<ContainerT>::TrySet
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.try_emplace(key_type{aKey}, aGenerateFn());

    return {iter->second, isPlacedNew};
}

template<typename ContainerT>
template<typename K,
         typename R>
std::tuple<R, bool> GpDictionary<ContainerT>::TrySet
(
    const K&            aKey,
    ValueGenerateFnT    aGenerateFn,
    ValueOnGetFnT<R>    aOnGetFn
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto [iter, isPlacedNew] = iContainer.try_emplace(key_type{aKey}, aGenerateFn());

    return {aOnGetFn(iter->second), isPlacedNew};
}

template<typename ContainerT>
template<typename K>
typename GpDictionary<ContainerT>::mapped_type  GpDictionary<ContainerT>::Erase (const K& aKey)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    THROW_COND_GP
    (
        iter != std::end(iContainer),
        [&]()
        {
            return fmt::format
            (
                "Element not found by key '{}'",
                std::to_string(aKey)
            );
        }
    );

    mapped_type val = std::move(iter->second);
    iContainer.erase(iter);

    return val;
}

template<typename ContainerT>
template<typename K>
std::optional<typename GpDictionary<ContainerT>::mapped_type>   GpDictionary<ContainerT>::EraseOpt (const K& aKey)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto iter = iContainer.find(aKey);

    if (iter == std::end(iContainer))
    {
        return std::nullopt;
    }

    mapped_type val = std::move(iter->second);
    iContainer.erase(iter);

    return val;
}

template<typename ContainerT>
GpDictionary<ContainerT>::this_type GpDictionary<ContainerT>::ExtractAll (void) noexcept
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    return this_type(std::move(*this));
}

template<typename ContainerT>
void    GpDictionary<ContainerT>::ProcessContainer (std::function<void(ContainerT&)> aFn)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    aFn(iContainer);
}

template<typename ContainerT>
void    GpDictionary<ContainerT>::ApplyToAll (std::function<void(mapped_type&)> aFn)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    for (auto& e: iContainer)
    {
        aFn(e.second);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
