#pragma once

#include "../../Macro/GpMacroClass.hpp"
#include "../../Concepts/GpConcepts.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace GPlatform{

class GpMath;

using Math = GpMath;

class GpMath
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpMath)

public:
    template<Concepts::IsFloatingPoint T>
    static T    SDegToRad   (const T aDeg) noexcept;

    template<Concepts::IsFloatingPoint T>
    static T    SRadToDeg   (const T aRad) noexcept;
};

template<Concepts::IsFloatingPoint T>
T   GpMath::SDegToRad (const T aDeg) noexcept
{
    return aDeg*(T(M_PI)/T(180.0));
}

template<Concepts::IsFloatingPoint T>
T   GpMath::SRadToDeg (const T aRad) noexcept
{
    return aRad*(T(180.0)/T(M_PI));
}

}//namespace GPlatform
