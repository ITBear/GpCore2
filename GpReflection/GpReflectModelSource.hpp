#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectModel.hpp>

namespace GPlatform {

class GpReflectModelSource
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectModelSource)
    CLASS_DD(GpReflectModelSource)

public:
                                        GpReflectModelSource    (void) noexcept = default;
    virtual                             ~GpReflectModelSource   (void) noexcept = default;

    virtual GpReflectModel::C::Opt::CSP Get                     (const GpUUID& aModelUid) = 0;
};

}// namespace GPlatform
