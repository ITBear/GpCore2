#pragma once

#include <GpCore2/GpUtils/Concepts/GpConcepts.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>
#include <vector>

namespace GPlatform {

template<Concepts::IsIntegral T>
class GpIdPool
{
public:
    using this_type     = GpIdPool<T>;
    using value_type    = T;
    using ReleasePoolT  = std::vector<value_type>;

public:
                    GpIdPool    (void) noexcept = default;
                    ~GpIdPool   (void) noexcept = default;

    value_type      Acquire     (void) noexcept;
    void            Release     (value_type aValue);

public:
    ReleasePoolT    iReleasePool;
    value_type      iCurrentMax = {};
};

template<Concepts::IsIntegral T>
auto    GpIdPool<T>::Acquire (void) noexcept -> value_type
{
    value_type value;

    // Try to find in release pool
    if (!iReleasePool.empty())
    {
        value = iReleasePool.back();
        iReleasePool.pop_back();
    } else
    {
        value       = iCurrentMax;
        iCurrentMax = NumOps::SInc(iCurrentMax);
    }

    return value;
}

template<Concepts::IsIntegral T>
void    GpIdPool<T>::Release (const value_type aValue)
{
    iReleasePool.push_back(aValue);
}

}// namespace GPlatform
