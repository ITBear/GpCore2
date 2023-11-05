#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../../SyncPrimitives/GpRWSpinLock.hpp"

#include <set>
#include <mutex>
#include <shared_mutex>

namespace GPlatform {

template <typename T>
class GpSet
{
public:
    CLASS_DD(GpSet<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::set<value_type, std::less<>>;
    using value_ref_opt         = std::optional<std::reference_wrapper<const value_type>>;
    using value_opt             = std::optional<value_type>;

public:
                                GpSet               (void) noexcept;
                                GpSet               (const GpSet& aSet) = delete;
                                GpSet               (GpSet&& aSet) noexcept;
                                ~GpSet              (void) noexcept;

    size_t                      Size                (void) const noexcept;
    bool                        Empty               (void) const noexcept;
    value_ref_opt               Find                (const value_type& aValue) const noexcept;
    value_opt                   ExtractNext         (void) noexcept;
    bool                        Emplace             (value_type aValue);
    void                        Set                 (GpSet&& aSet) noexcept;
    GpSet&                      operator=           (GpSet&& aSet) noexcept;

    //unsafe
    underlying_container&       UnderlyingContainer (void) noexcept;
    const underlying_container& UnderlyingContainer (void) const noexcept;

private:
    mutable GpRWSpinLock        iLockRW;
    underlying_container        iContainer;
};

template <typename T>

GpSet<T>::GpSet (void) noexcept
{
}

template <typename T>
GpSet<T>::~GpSet (void) noexcept
{
}

template <typename T>
size_t  GpSet<T>::Size (void) const noexcept
{
    std::shared_lock lockW(iLockRW);

    return iContainer.size();
}

template <typename T>
bool    GpSet<T>::Empty (void) const noexcept
{
    std::shared_lock lockW(iLockRW);

    return iContainer.empty();
}

template <typename T>
typename GpSet<T>::value_ref_opt    GpSet<T>::Find (const value_type& aValue) const noexcept
{
    std::shared_lock lockW(iLockRW);

    auto iter = iContainer.find(aValue);

    if (iter == iContainer.end())
    {
        return std::nullopt;
    } else
    {
        return *iter;
    }
}

template <typename T>
typename GpSet<T>::value_opt    GpSet<T>::ExtractNext (void) noexcept
{
    std::scoped_lock lockRW(iLockRW);

    auto iter = iContainer.begin();

    if (iter == iContainer.end())
    {
        return std::nullopt;
    } else
    {
        return std::move(iContainer.extract(iter).value());
    }
}

template <typename T>
bool    GpSet<T>::Emplace (value_type aValue)
{
    std::scoped_lock lockRW(iLockRW);

    return iContainer.emplace(std::move(aValue)).second;
}

template <typename T>
void    GpSet<T>::Set (GpSet&& aSet) noexcept
{
    if (this == &aSet)
    {
        return;
    }

    std::scoped_lock lock1(iLockRW);
    std::scoped_lock lock2(aSet.iLockRW);

    iContainer = std::move(aSet.iContainer);
}

template <typename T>
GpSet<T>&   GpSet<T>::operator= (GpSet&& aSet) noexcept
{
    Set(std::move(aSet));
    return *this;;
}

template <typename T>
typename GpSet<T>::underlying_container&    GpSet<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpSet<T>::underlying_container&  GpSet<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
