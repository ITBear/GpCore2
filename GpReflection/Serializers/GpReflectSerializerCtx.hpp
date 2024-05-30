#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform {

class GpReflectSerializerCtx
{
public:
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectSerializerCtx)
    CLASS_DD(GpReflectSerializerCtx)

protected:
                        GpReflectSerializerCtx  (void) noexcept = default;

public:
    virtual             ~GpReflectSerializerCtx (void) noexcept = default;
};

}// namespace GPlatform
