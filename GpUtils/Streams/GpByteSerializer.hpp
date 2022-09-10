#pragma once

#include "GpByteWriter.hpp"
#include <any>

namespace GPlatform {

class GpByteSerializer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpByteSerializer)
    CLASS_DECLARE_DEFAULTS(GpByteSerializer)

protected:
                            GpByteSerializer    (void) noexcept = default;

public:
    virtual                 ~GpByteSerializer   (void) noexcept = default;

    virtual void            Serialize           (const std::any&    aObject,
                                                 GpByteWriter&      aWriter) const = 0;
};

}//namespace GPlatform
