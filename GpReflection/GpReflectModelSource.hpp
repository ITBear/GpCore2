#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectModel.hpp"

namespace GPlatform {

class GpReflectModelSource
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectModelSource)
    CLASS_DD(GpReflectModelSource)

public:
                                        GpReflectModelSource    (void) noexcept = default;
    virtual                             ~GpReflectModelSource   (void) noexcept = default;

    virtual GpReflectModel::C::Opt::Val Get                     (const GpUUID& aModelUid) = 0;
};

}//GPlatform

#endif//GP_USE_REFLECTION
