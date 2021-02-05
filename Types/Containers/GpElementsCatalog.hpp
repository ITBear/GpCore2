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
         typename ValueT,
         template<typename...> class ContainerT>
class GpElementsCatalog
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpElementsCatalog)

public:
    using this_type         = GpElementsCatalog<KeyT, ValueT, ContainerT>;
    using key_type          = KeyT;
    using value_type        = ValueT;
    using container_type    = ContainerT<KeyT, ValueT, std::less<>>;
    using RegisterResT      = std::optional<std::reference_wrapper<ValueT>>;

    CLASS_TAG(THREAD_SAFE)

public:
                            GpElementsCatalog       (void) noexcept = default;
    virtual                 ~GpElementsCatalog      (void) noexcept;

    void                    Clear                   (void) noexcept;

    const ValueT&           FindOrRegister          (const KeyT&    aKey, std::function<ValueT()> aValueFn);
    const ValueT&           FindOrRegister          (KeyT&&         aKey, std::function<ValueT()> aValueFn);

    template<typename T>
    const ValueT&           FindOrRegister          (T aKey, std::function<ValueT()> aValueFn);

    void                    Register                (const KeyT& aKey, const ValueT& aValue);
    void                    Register                (KeyT&& aKey, const ValueT& aValue);
    void                    Register                (const KeyT& aKey, ValueT&& aValue);
    void                    Register                (KeyT&& aKey, ValueT&& aValue);

    RegisterResT            TryRegister             (const KeyT& aKey, const ValueT& aValue);
    RegisterResT            TryRegister             (KeyT&& aKey, const ValueT& aValue);
    RegisterResT            TryRegister             (const KeyT& aKey, ValueT&& aValue);
    RegisterResT            TryRegister             (KeyT&& aKey, ValueT&& aValue);

    template<typename T>
    RegisterResT            TryRegister             (T aKey, const ValueT& aValue);

    template<typename T>
    RegisterResT            TryRegister             (T aKey, ValueT&& aValue);

    void                    ReplaceOrRegister       (const KeyT& aKey, const ValueT& aValue);
    void                    ReplaceOrRegister       (KeyT&& aKey, const ValueT& aValue);
    void                    ReplaceOrRegister       (const KeyT& aKey, ValueT&& aValue);
    void                    ReplaceOrRegister       (KeyT&& aKey, ValueT&& aValue);

    ValueT                  Unregister              (const KeyT& aKey);

    template<typename T>
    ValueT                  Unregister              (T aKey);

    std::optional<std::reference_wrapper<const ValueT>>
                            Find                    (const KeyT& aKey) const noexcept;

    template<typename T>
    std::optional<std::reference_wrapper<const ValueT>>
                            Find                    (T aKey) const noexcept;

    std::optional<std::reference_wrapper<ValueT>>
                            Find                    (const KeyT& aKey) noexcept;
    template<typename T>
    std::optional<std::reference_wrapper<ValueT>>
                            Find                    (T aKey) noexcept;

    std::optional<ValueT>   FindRetCopy             (const KeyT& aKey) const noexcept;

    template<typename T>
    std::optional<ValueT>   FindRetCopy             (T aKey) const noexcept;

    const ValueT&           FindOrThrow             (const KeyT& aKey) const;

    template<typename T>
    const ValueT&           FindOrThrow             (T aKey) const;

    ValueT&                 FindOrThrow             (const KeyT& aKey);

    template<typename T>
    ValueT&                 FindOrThrow             (T aKey);

    void                    Process                 (std::function<void(container_type&)> aFn);

private:
    count_t                 _Count                  (const KeyT& aKey) const noexcept;

private:
    mutable GpRWLock        iLock;
    container_type          iElements;
};

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
GpElementsCatalog<KeyT, ValueT, ContainerT>::~GpElementsCatalog (void) noexcept
{
    Clear();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Clear (void) noexcept
{
    std::scoped_lock lock(iLock);

    iElements.clear();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
const ValueT&   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrRegister (const KeyT& aKey, std::function<ValueT()> aValueFn)
{
    const auto val = Find(aKey);
    if (val.has_value()) return val.value().get();

    auto res = TryRegister(aKey, aValueFn());

    if (res.has_value()) return res.value().get();
    else return Find(aKey).value().get();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
const ValueT&   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrRegister (KeyT&& aKey, std::function<ValueT()> aValueFn)
{
    const auto val = Find(aKey);
    if (val.has_value()) return val.value().get();

    auto res = TryRegister(std::move(aKey), aValueFn());

    if (res.has_value()) return res.value().get();
    else return Find(aKey).value().get();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
const ValueT&   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrRegister (T aKey, std::function<ValueT()> aValueFn)
{
    const auto val = Find(aKey);
    if (val.has_value()) return val.value().get();

    auto res = TryRegister(aKey, aValueFn());

    if (res.has_value()) return res.value().get();
    else return Find(aKey).value().get();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (const KeyT& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + StrOps::SToString(aKey) + "' is not unique"_sv);

    iElements.try_emplace(aKey, aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (KeyT&& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + StrOps::SToString(aKey) + "' is not unique"_sv);

    iElements.try_emplace(std::move(aKey), aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (const KeyT& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + StrOps::SToString(aKey) + "' is not unique"_sv);

    iElements.try_emplace(aKey, std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (KeyT&& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + StrOps::SToString(aKey) + "' is not unique"_sv);

    iElements.try_emplace(std::move(aKey), std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (const KeyT& aKey, const ValueT& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(aKey, aValue);

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (KeyT&& aKey, const ValueT& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(std::move(aKey), aValue);

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (const KeyT& aKey, ValueT&& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(aKey, std::move(aValue));

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (KeyT&& aKey, ValueT&& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(std::move(aKey), std::move(aValue));

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (T aKey, const ValueT& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(KeyT(aKey), aValue);

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
auto    GpElementsCatalog<KeyT, ValueT, ContainerT>::TryRegister (T aKey, ValueT&& aValue) -> RegisterResT
{
    std::scoped_lock lock(iLock);

    auto res = iElements.try_emplace(KeyT(aKey), std::move(aValue));

    if (res.second)
    {
        return res.first->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::ReplaceOrRegister (const KeyT& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    iElements.insert_or_assign(aKey, aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::ReplaceOrRegister (KeyT&& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    iElements.insert_or_assign(std::move(aKey), aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::ReplaceOrRegister (const KeyT& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    iElements.insert_or_assign(aKey, std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::ReplaceOrRegister (KeyT&& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    iElements.insert_or_assign(std::move(aKey), std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
ValueT  GpElementsCatalog<KeyT, ValueT, ContainerT>::Unregister (const KeyT& aKey)
{
    return Unregister<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
ValueT  GpElementsCatalog<KeyT, ValueT, ContainerT>::Unregister (T aKey)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        ValueT v = std::move(iter->second);
        iElements.erase(iter);
        return v;
    }

    THROW_GPE("Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
std::optional<std::reference_wrapper<const ValueT>> GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (const KeyT& aKey) const noexcept
{
    return Find<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
std::optional<std::reference_wrapper<const ValueT>> GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (T aKey) const noexcept
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        //return std::cref(iter->second);
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
std::optional<std::reference_wrapper<ValueT>>   GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (const KeyT& aKey) noexcept
{
    return Find<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
std::optional<std::reference_wrapper<ValueT>>   GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (T aKey) noexcept
{
    auto res = std::as_const(*this).template Find<T>(aKey);

    if (res.has_value())
    {
        return const_cast<ValueT&>(res.value().get());
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
std::optional<ValueT>   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindRetCopy (const KeyT& aKey) const noexcept
{
    return FindRetCopy<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
std::optional<ValueT>   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindRetCopy    (T aKey) const noexcept
{
    std::shared_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        return std::cref(iter->second);
    } else
    {
        return std::nullopt;
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
const ValueT&   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrThrow (const KeyT& aKey) const
{
    return FindOrThrow<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
const ValueT&   GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrThrow (T aKey) const
{
    auto res = Find<T>(aKey);
    THROW_GPE_COND_CHECK_M(res.has_value(),
                           "Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv);
    return res.value().get();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
ValueT& GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrThrow (const KeyT& aKey)
{
    return FindOrThrow<const KeyT&>(aKey);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
template<typename T>
ValueT& GpElementsCatalog<KeyT, ValueT, ContainerT>::FindOrThrow (T aKey)
{
    auto res = Find<T>(aKey);
    THROW_GPE_COND_CHECK_M(res.has_value(),
                           "Element not found by key '"_sv + StrOps::SToString(aKey) + "'"_sv);
    return res.value().get();
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Process (std::function<void(container_type&)> aFn)
{
    std::shared_lock lock(iLock);
    aFn(iElements);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
count_t GpElementsCatalog<KeyT, ValueT, ContainerT>::_Count (const KeyT& aKey) const noexcept
{
    return count_t::SMake(iElements.count(aKey));
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
