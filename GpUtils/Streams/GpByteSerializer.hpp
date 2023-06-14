#pragma once

#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/Containers/GpAny.hpp"
#include "GpByteWriter.hpp"

namespace GPlatform {

class GpByteSerializer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpByteSerializer)
    CLASS_DD(GpByteSerializer)

protected:
                            GpByteSerializer    (void) noexcept = default;

public:
    virtual                 ~GpByteSerializer   (void) noexcept = default;

    virtual void            Serialize           (const GpAny&   aObject,
                                                 GpByteWriter&  aWriter) const = 0;
};

}//namespace GPlatform
