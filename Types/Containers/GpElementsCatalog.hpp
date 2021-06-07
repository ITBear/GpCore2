#pragma once

#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../Units/Other/count_t.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../../Multithreading/SyncPrimitives/GpSyncPrimitives.hpp"

#include <mutex>
#include <shared_mutex>
#include <functional>

namespace GPlatform {

template<typename KeyT,
         typename ValueT>
class GpElementsCatalog
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpElementsCatalog)

public:
    using this_type             = GpElementsCatalog<KeyT, ValueT>;
    using key_type              = KeyT;
    using value_type            = ValueT;
    using container_type        = GpMap<KeyT, ValueT, std::less<>>;
    using ValueRefOptT          = std::optional<std::reference_wrapper<ValueT>>;
    using ValueCRefOptT         = std::optional<std::reference_wrapper<const ValueT>>;
    using ValueOptT             = std::optional<ValueT>;
    using ValueCOptT            = std::optional<const ValueT>;

    template<typename T>
    struct IsReferenceWrapper : std::false_type {};

    template<typename T>
    struct IsReferenceWrapper<std::reference_wrapper<T>> : std::true_type{};

    CLASS_TAG(THREAD_SAFE)

public:
                            GpElementsCatalog       (void) noexcept = default;
    virtual                 ~GpElementsCatalog      (void) noexcept;

    void                    Clear                   (void) noexcept;

    template<typename K>
    ValueT&                 FindOrRegister          (K&& aKey, std::function<ValueT()> aValueFn);

    template<typename K, typename V>
    void                    Register                (K&& aKey, V&& aValue);

    template<typename K, typename V>
    ValueRefOptT            TryRegister             (K&& aKey, V&& aValue);

    template<typename K, typename V>
    void                    ReplaceOrRegister       (K&& aKey, V&& aValue);

    template<typename K>
    ValueT                  Unregister              (K&& aKey);

    template<typename K>
    const ValueT&           Find                    (K&& aKey) const;

    template<typename K>
    ValueT&                 Find                    (K&& aKey);

    template<typename K>
    ValueCRefOptT           TryFind                 (K&& aKey) const noexcept;

    template<typename K>
    ValueRefOptT            TryFind                 (K&& aKey) noexcept;

    template<typename K>
    ValueOptT               TryFindRetCopy          (K&& aKey) const;

    void                    Process                 (std::function<void(container_type&)> aFn);
    void                    Apply                   (std::function<void(ValueT&)> aFn);

private:
    template<typename K>
    count_t                 _Count                  (K&& aKey) const noexcept;

private:
    mutable GpRWLock        iLock;
    container_type          iElements;
};

template<typename KeyT,
         typename ValueT>
GpElementsCatalog<KeyT, ValueT>::~GpElementsCatalog (void) noexcept
{
    Clear();
}

template<typename KeyT,
         typename ValueT>
void    GpElementsCatalog<KeyT, ValueT>::Clear (void) noexcept
{
    std::scoped_lock lock(iLock);
    iElements.clear();
}

template<typename KeyT,
         typename ValueT>
template<typename K>
ValueT& GpElementsCatalog<KeyT, ValueT>::FindOrRegister
(
    K&&                     aKey,
    std::function<ValueT()> aValueFn
)
{
    const auto val = TryFind<K>(std::forward<K>(aKey));
    if (val.has_value())
    {
        return val.value().get();
    }

    auto reg = TryRegister<K>(std::forward<K>(aKey), aValueFn());

    if (reg.has_value())
    {
        return reg.value().get();
    } else
    {
        return TryFind<K>(std::forward<K>(aKey)).value().get();
    }
}

template<typename KeyT,
         typename ValueT>
template<typename K,
         typename V>
void    GpElementsCatalog<KeyT, ValueT>::Register
(
    K&& aKey,
    V&& aValue
)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND
    (
        _Count<K>(std::forward<K>(aKey)) == 0_cnt,
        [&](){return "Key '"_sv + StrOps::SToString(std::forward<K>(aKey)) + "' is not unique"_sv;}
    );

    iElements.emplace(std::forward<K>(aKey), std::forward<V>(aValue));
}

template<typename KeyT,
         typename ValueT>
template<typename K,
         typename V>
auto    GpElementsCatalog<KeyT, ValueT>::TryRegister
(
    K&& aKey,
    V&& aValue
) -> ValueRefOptT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.emplace(std::forward<K>(aKey), std::forward<V>(aValue));

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }

    return std::nullopt;
}

template<typename KeyT,
         typename ValueT>
template<typename K,
         typename V>
void    GpElementsCatalog<KeyT, ValueT>::ReplaceOrRegister
(
    K&& aKey,
    V&& aValue
)
{
    std::scoped_lock lock(iLock);
    iElements.insert_or_assign(std::forward<K>(aKey), std::forward<V>(aValue));
}

template<typename KeyT,
         typename ValueT>
template<typename K>
ValueT  GpElementsCatalog<KeyT, ValueT>::Unregister (K&& aKey)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(std::forward<K>(aKey));

    THROW_GPE_COND
    (
        iter != iElements.end(),
        [&](){return "Element not found by key '"_sv + StrOps::SToString(std::forward<K>(aKey)) + "'"_sv;}
    );

    ValueT v = std::move(iter->second);
    iElements.erase(iter);
    return v;
}

template<typename KeyT,
         typename ValueT>
template<typename K>
const ValueT&   GpElementsCatalog<KeyT, ValueT>::Find (K&& aKey) const
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(std::forward<K>(aKey));

    THROW_GPE_COND
    (
        iter != iElements.end(),
        [&](){return "Element not found by key '"_sv + StrOps::SToString(std::forward<K>(aKey)) + "'"_sv;}
    );

    return iter->second;
}

template<typename KeyT,
         typename ValueT>
template<typename K>
ValueT& GpElementsCatalog<KeyT, ValueT>::Find (K&& aKey)
{
    return const_cast<ValueT&>(std::as_const(*this).template Find<K>(std::forward<K>(aKey)));
}

template<typename KeyT,
         typename ValueT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT>::TryFind (K&& aKey) const noexcept -> ValueCRefOptT
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(std::forward<K>(aKey));

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
         typename ValueT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT>::TryFind (K&& aKey) noexcept -> ValueRefOptT
{
    auto res = std::as_const(*this).template TryFind<K>(std::forward<K>(aKey));

    if (res.has_value())
    {
        return const_cast<ValueT&>(res.value().get());
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT>
template<typename K>
auto    GpElementsCatalog<KeyT, ValueT>::TryFindRetCopy (K&& aKey) const -> ValueOptT
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(std::forward<K>(aKey));

    if (iter != iElements.end())
    {
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT>
void    GpElementsCatalog<KeyT, ValueT>::Process (std::function<void(container_type&)> aFn)
{
    std::scoped_lock lock(iLock);
    aFn(iElements);
}

template<typename KeyT,
         typename ValueT>
void    GpElementsCatalog<KeyT, ValueT>::Apply (std::function<void(ValueT&)> aFn)
{
    std::shared_lock lock(iLock);

    for (auto& e: iElements)
    {
        aFn(e.second);
    }
}

template<typename KeyT,
         typename ValueT>
template<typename K>
count_t GpElementsCatalog<KeyT, ValueT>::_Count (K&& aKey) const noexcept
{
    return count_t::SMake(iElements.count(std::forward<K>(aKey)));
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
