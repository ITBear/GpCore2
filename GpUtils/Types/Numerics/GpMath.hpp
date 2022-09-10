#pragma once

#include "../../Types/Numerics/GpNumericOps.hpp"

namespace GPlatform{

class GpMath;

using Math = GpMath;

class GpMath
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpMath)

public:
    template<typename T>
    requires std::floating_point<T>
    static T    SDegToRad   (const T aDeg) noexcept;

    template<typename T>
    requires std::floating_point<T>
    static T    SRadToDeg   (const T aRad) noexcept;
};

template<typename T>
requires std::floating_point<T>
T   GpMath::SDegToRad (const T aDeg) noexcept
{
    return aDeg*(T(M_PI)/T(180.0));
}

template<typename T>
requires std::floating_point<T>
T   GpMath::SRadToDeg (const T aRad) noexcept
{
    return aRad*(T(180.0)/T(M_PI));
}

}//namespace GPlatform
