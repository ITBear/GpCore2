#pragma once

#pragma once

#include "../../GpUtils_global.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../../SyncPrimitives/GpSyncPrimitives.hpp"

#include <mutex>
#include <shared_mutex>
#include <functional>

namespace GPlatform {

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT = GpMap<KeyT, ValueT, std::less<>>>
class GpElementsCatalog
{
    CLASS_REMOVE_CTRS_COPY(GpElementsCatalog)

public:
    using this_type         = GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>;
    using key_type          = KeyT;
    using value_type        = ValueT;
    using container_type    = UnderlyingContainerT;
    using ValueRefOptT      = std::optional<std::reference_wrapper<ValueT>>;
    using ValueCRefOptT     = std::optional<std::reference_wrapper<const ValueT>>;
    using ValueOptT         = std::optional<ValueT>;
    using ValueGenFnT       = std::function<ValueT()>;
    using ValueUpdateFnT    = std::function<void(ValueT&)>;

    template<typename T>
    struct IsReferenceWrapper: std::false_type {};

    template<typename T>
    struct IsReferenceWrapper<std::reference_wrapper<T>> : std::true_type{};

    CLASS_TAG(THREAD_SAFE)

public:
                            GpElementsCatalog   (GpElementsCatalog&& aCatalog) noexcept;
                            GpElementsCatalog   (void) noexcept = default;
    virtual                 ~GpElementsCatalog  (void) noexcept;

    size_t                  Size                (void) const noexcept;

    void                    Clear               (void) noexcept;

    template<typename K>
    const ValueT&           Find                (K&& aKey) const;

    template<typename K>
    ValueT&                 Find                (K&& aKey);

    template<typename K>
    ValueCRefOptT           FindOpt             (K&& aKey) const noexcept;

    template<typename K>
    ValueRefOptT            FindOpt             (K&& aKey) noexcept;

    template<typename K>
    ValueT                  FindRetCopy         (K&& aKey) const;

    template<typename K>
    ValueOptT               FindRetCopyOpt      (K&& aKey) const noexcept;

    template<typename K, typename V>
    void                    Register            (K&& aKey,
                                                 V&& aValue);

    template<typename K, typename V>
    [[nodiscard]] bool      TryRegister         (K&& aKey,
                                                 V&& aValue);

    template<typename K>
    ValueT&                 FindOrRegister      (K&&            aKey,
                                                 ValueGenFnT    aGenFn);

    template<typename K>
    ValueT&                 RegisterOrUpdate    (K&&            aKey,
                                                 ValueGenFnT    aGenFn,
                                                 ValueUpdateFnT aUpdateFn);

    template<typename K>
    ValueT                  Unregister          (K&& aKey);

    template<typename K>
    ValueOptT               UnregisterOpt       (K&& aKey);

    this_type               UnregisterAll       (void) noexcept;

    void                    Process             (std::function<void(container_type&)> aFn);
    void                    Apply               (std::function<void(ValueT&)> aFn);

private:
    mutable GpRWLock        iLock;
    container_type          iElements;
};

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::GpElementsCatalog (GpElementsCatalog&& aCatalog) noexcept
{
    iElements = std::move(aCatalog.iElements);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::~GpElementsCatalog (void) noexcept
{
    Clear();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
size_t  GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Size (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iElements.size();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Clear (void) noexcept
{
    std::scoped_lock lock(iLock);
    iElements.clear();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
const ValueT&   GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Find (K&& aKey) const
{
    return const_cast<this_type&>(*this).Find<K>(aKey);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT& GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Find (K&& aKey)
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return "Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv;}
    );

    return iter->second;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::FindOpt (K&& aKey) const noexcept -> ValueCRefOptT
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        if constexpr (IsReferenceWrapper<ValueT>::value)
        {
            return iter->second;
        } else
        {
            return std::cref(iter->second);
        }
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::FindOpt (K&& aKey) noexcept -> ValueRefOptT
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        if constexpr (IsReferenceWrapper<ValueT>::value)
        {
            return iter->second;
        } else
        {
            return std::ref(iter->second);
        }
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT  GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::FindRetCopy (K&& aKey) const
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return "Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv;}
    );

    return ValueT(iter->second);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::FindRetCopyOpt (K&& aKey) const noexcept -> ValueOptT
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        return ValueT(iter->second);
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K,
         typename V>
void    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Register
(
    K&& aKey,
    V&& aValue
)
{
    THROW_COND_GP
    (
        TryRegister
        (
            std::forward<K>(aKey),
            std::forward<V>(aValue)
        ),
        [&](){return "Key '"_sv + StrOps::SToString(aKey) + "' is not unique"_sv;}
    );
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K,
         typename V>
bool    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::TryRegister
(
    K&& aKey,
    V&& aValue
)
{
    bool isInserted = false;
    {
        std::scoped_lock lock(iLock);
        isInserted = iElements.try_emplace
        (
            KeyT(std::forward<K>(aKey)),
            std::forward<V>(aValue)
        ).second;
    }

    return isInserted;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT& GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::FindOrRegister
(
    K&&         aKey,
    ValueGenFnT aGenFn
)
{
    //Try find (shared lock)
    {
        std::shared_lock lock(iLock);
        auto iter = iElements.find(aKey);
        if (iter != iElements.end())
        {
            return iter->second;
        }
    }

    //Try find or register (scoped lock)
    {
        //Try find
        std::scoped_lock lock(iLock);
        auto iter = iElements.find(aKey);
        if (iter != iElements.end())
        {
            return iter->second;
        }

        //Register
        return iElements.try_emplace
        (
            KeyT(std::forward<K>(aKey)),
            aGenFn()
        ).first->second;
    }
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT& GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::RegisterOrUpdate
(
    K&&             aKey,
    ValueGenFnT     aGenFn,
    ValueUpdateFnT  aUpdateFn
)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())//Update
    {
        aUpdateFn(iter->second);
        return iter->second;
    } else
    {
        return iElements.try_emplace
        (
            KeyT(std::forward<K>(aKey)),
            aGenFn()
        ).first->second;
    }
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT  GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Unregister (K&& aKey)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return "Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv;}
    );

    ValueT val = std::move(iter->second);
    iElements.erase(iter);

    return val;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::UnregisterOpt (K&& aKey) -> ValueOptT
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter == iElements.end())
    {
        return std::nullopt;
    }

    ValueT val = std::move(iter->second);
    iElements.erase(iter);

    return val;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
auto    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::UnregisterAll (void) noexcept -> this_type
{
    std::scoped_lock lock(iLock);
    return this_type(std::move(*this));
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Process (std::function<void(container_type&)> aFn)
{
    std::scoped_lock lock(iLock);
    aFn(iElements);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpElementsCatalog<KeyT, ValueT, UnderlyingContainerT>::Apply (std::function<void(ValueT&)> aFn)
{
    std::shared_lock lock(iLock);

    for (auto& e: iElements)
    {
        aFn(e.second);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
