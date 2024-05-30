#pragma once

#include "GpByteSerializer.hpp"

namespace GPlatform {

class GpByteSerializerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpByteSerializerFactory)
    CLASS_DD(GpByteSerializerFactory)

public:
                                    GpByteSerializerFactory     (void) noexcept = default;
    virtual                         ~GpByteSerializerFactory    (void) noexcept = default;

    virtual GpByteSerializer::SP    NewInstance                 (void) const = 0;
};

}// namespace GPlatform
