#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../../SyncPrimitives/GpRWLock.hpp"

#include <set>
#include <mutex>
#include <shared_mutex>

namespace GPlatform {

template <typename T>
class GpSet
{
public:
    using value_type            = T;
    using this_type             = GpSet<value_type>;
    using internal_container    = std::set<value_type>;
    using value_ref_opt         = std::optional<std::reference_wrapper<const value_type>>;
    using value_opt             = std::optional<value_type>;

public:
                            GpSet       (void) noexcept;
                            GpSet       (const GpSet& aSet) = delete;
                            GpSet       (GpSet&& aSet) noexcept;
                            ~GpSet      (void) noexcept;

    value_ref_opt           Find        (const value_type& aValue) const noexcept;
    value_opt               ExtractNext (void) noexcept;

    bool                    Emplace     (value_type aValue);

private:
    mutable GpRWLock        iLockRW;
    internal_container      iContainer;
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

}//namespace GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
