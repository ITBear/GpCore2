#pragma once

#include "../GpMacro.hpp"

#include <functional>

namespace GPlatform {

template<typename FnT>
class GpRAIIonDestruct
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpRAIIonDestruct)

public:
                GpRAIIonDestruct    (FnT aFn) noexcept: iFn(aFn) {}
                ~GpRAIIonDestruct   (void) noexcept {iFn();}

private:
    FnT         iFn;
};

}//GPlatform
