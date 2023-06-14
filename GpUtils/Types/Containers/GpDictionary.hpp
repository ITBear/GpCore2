#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Strings/GpStringOps.hpp"
#include "../../SyncPrimitives/GpRWLock.hpp"

#include <mutex>
#include <shared_mutex>
#include <functional>

namespace GPlatform {

TAG_REGISTER(GpDictionary)

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT = std::map<KeyT, ValueT, std::less<>>>
class GpDictionary
{
    CLASS_REMOVE_CTRS_COPY(GpDictionary)

public:
    using this_type         = GpDictionary<KeyT, ValueT, UnderlyingContainerT>;
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

    TAG_SET(GpDictionary)
    TAG_SET(THREAD_SAFE)

public:
                            GpDictionary    (GpDictionary&& aCatalog) noexcept;
                            GpDictionary    (void) noexcept = default;
    virtual                 ~GpDictionary   (void) noexcept;

    size_t                  Size            (void) const noexcept;

    void                    Clear           (void) noexcept;

    template<typename K>
    const ValueT&           Get             (K&& aKey) const;

    template<typename K>
    ValueT&                 Get             (K&& aKey);

    template<typename K>
    ValueCRefOptT           GetOpt          (K&& aKey) const noexcept;

    template<typename K>
    ValueRefOptT            GetOpt          (K&& aKey) noexcept;

    template<typename K>
    ValueT                  GetCopy         (K&& aKey) const;

    template<typename K>
    ValueOptT               GetCopyOpt      (K&& aKey) const;

    template<typename K, typename V>
    void                    Set             (K&& aKey,
                                             V&& aValue);

    template<typename K, typename V>
    [[nodiscard]] bool      TrySet          (K&& aKey,
                                             V&& aValue);

    template<typename K>
    ValueT&                 GetOrSet        (K&&            aKey,
                                             ValueGenFnT    aGenFn);

    template<typename K>
    ValueT&                 UpdateOrSet     (K&&            aKey,
                                             ValueGenFnT    aGenFn,
                                             ValueUpdateFnT aUpdateFn);

    template<typename K>
    ValueT                  Erase           (K&& aKey);

    template<typename K>
    ValueOptT               EraseOpt        (K&& aKey);

    this_type               EraseAll        (void) noexcept;

    void                    Process         (std::function<void(container_type&)> aFn);
    void                    Apply           (std::function<void(ValueT&)> aFn);

private:
    mutable GpRWLock        iLock;
    container_type          iElements;
};

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GpDictionary (GpDictionary&& aCatalog) noexcept:
iElements(std::move(aCatalog.iElements))
{   
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
GpDictionary<KeyT, ValueT, UnderlyingContainerT>::~GpDictionary (void) noexcept
{
    Clear();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
size_t  GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Size (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iElements.size();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Clear (void) noexcept
{
    std::scoped_lock lock(iLock);
    iElements.clear();
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
const ValueT&   GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Get (K&& aKey) const
{
    return const_cast<this_type&>(*this).Get<K>(aKey);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
ValueT& GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Get (K&& aKey)
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv;}
    );

    return iter->second;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GetOpt (K&& aKey) const noexcept -> ValueCRefOptT
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
auto    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GetOpt (K&& aKey) noexcept -> ValueRefOptT
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
ValueT  GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GetCopy (K&& aKey) const
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv;}
    );

    return ValueT(iter->second);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GetCopyOpt (K&& aKey) const -> ValueOptT
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
void    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Set
(
    K&& aKey,
    V&& aValue
)
{
    THROW_COND_GP
    (
        TrySet
        (
            std::forward<K>(aKey),
            std::forward<V>(aValue)
        ),
        [&](){return u8"Key '"_sv + StrOps::SToString(aKey) + u8"' is not unique"_sv;}
    );
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K,
         typename V>
bool    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::TrySet
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
ValueT& GpDictionary<KeyT, ValueT, UnderlyingContainerT>::GetOrSet
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

        //Set
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
ValueT& GpDictionary<KeyT, ValueT, UnderlyingContainerT>::UpdateOrSet
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
ValueT  GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Erase (K&& aKey)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    THROW_COND_GP
    (
        iter != iElements.end(),
        [&](){return u8"Element not found by key '"_sv + StrOps::SToString(aKey) + u8"'"_sv;}
    );

    ValueT val = std::move(iter->second);
    iElements.erase(iter);

    return val;
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
template<typename K>
auto    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::EraseOpt (K&& aKey) -> ValueOptT
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
auto    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::EraseAll (void) noexcept -> this_type
{
    std::scoped_lock lock(iLock);
    return this_type(std::move(*this));
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Process (std::function<void(container_type&)> aFn)
{
    std::scoped_lock lock(iLock);
    aFn(iElements);
}

template<typename KeyT,
         typename ValueT,
         typename UnderlyingContainerT>
void    GpDictionary<KeyT, ValueT, UnderlyingContainerT>::Apply (std::function<void(ValueT&)> aFn)
{
    std::shared_lock lock(iLock);

    for (auto& e: iElements)
    {
        aFn(e.second);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
