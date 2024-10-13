#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

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

}// namespace GPlatform
