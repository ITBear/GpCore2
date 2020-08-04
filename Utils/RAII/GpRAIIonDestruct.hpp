#pragma once

#include "../../Config/GpConfig.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

#include <functional>

namespace GPlatform {

template<typename FnT>
class GpRAIIonDestruct
{
    CLASS_REMOVE_CTRS(GpRAIIonDestruct)

public:
                GpRAIIonDestruct    (FnT aFn) noexcept: iFn(aFn) {}
                ~GpRAIIonDestruct   (void) noexcept {iFn();}

private:
    FnT         iFn;
};

}//GPlatform
