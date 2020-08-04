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
    using container_type    = ContainerT<KeyT, ValueT>;

    CLASS_TAG(THREAD_SAFE)

public:
                                    GpElementsCatalog   (void) noexcept = default;
    virtual                         ~GpElementsCatalog  (void) noexcept;

    void                            Clear               (void) noexcept;

    void                            Register            (const KeyT& aKey, const ValueT& aValue);
    void                            Register            (KeyT&& aKey, const ValueT& aValue);
    void                            Register            (const KeyT& aKey, ValueT&& aValue);
    void                            Register            (KeyT&& aKey, ValueT&& aValue);

    ValueT                          Unregister          (const KeyT& aKey);

    std::optional<std::reference_wrapper<const ValueT>>
                                    Find                (const KeyT& aKey) const noexcept;
    std::optional<std::reference_wrapper<ValueT>>
                                    Find                (const KeyT& aKey) noexcept;

private:
    count_t                         _Count              (const KeyT& aKey) const noexcept;

private:
    mutable GpRWLock                iLock;
    container_type                  iElements;
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
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (const KeyT& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + std::to_string(aKey) + "' is not unique"_sv);

    iElements.emplace(aKey, aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (KeyT&& aKey, const ValueT& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + std::to_string(aKey) + "' is not unique"_sv);

    iElements.emplace(std::move(aKey), aValue);
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (const KeyT& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + std::to_string(aKey) + "' is not unique"_sv);

    iElements.emplace(aKey, std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
void    GpElementsCatalog<KeyT, ValueT, ContainerT>::Register (KeyT&& aKey, ValueT&& aValue)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(_Count(aKey) == 0_cnt, "Key '"_sv + std::to_string(aKey) + "' is not unique"_sv);

    iElements.emplace(std::move(aKey), std::move(aValue));
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
ValueT  GpElementsCatalog<KeyT, ValueT, ContainerT>::Unregister (const KeyT& aKey)
{
    std::scoped_lock lock(iLock);

    auto iter = iElements.find(aKey);

    if (iter != iElements.end())
    {
        ValueT v = std::move(iter->second);
        iElements.erase(iter);
        return v;
    } else
    {
        THROW_GPE("Element not found by key '"_sv + std::to_string(aKey) + "'"_sv);
    }
}

template<typename KeyT,
         typename ValueT,
         template<typename...> class ContainerT>
std::optional<std::reference_wrapper<const ValueT>> GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (const KeyT& aKey) const noexcept
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
std::optional<std::reference_wrapper<ValueT>>   GpElementsCatalog<KeyT, ValueT, ContainerT>::Find (const KeyT& aKey) noexcept
{
    auto res = std::as_const(*this).Find(aKey);

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
count_t GpElementsCatalog<KeyT, ValueT, ContainerT>::_Count (const KeyT& aKey) const noexcept
{
    return count_t::SMake(iElements.count(aKey));
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
