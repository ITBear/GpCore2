#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeMapper.hpp"

namespace GPlatform {

class GpTypeMapperFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTypeMapperFactory)
    CLASS_DECLARE_DEFAULTS(GpTypeMapperFactory)

protected:
                                    GpTypeMapperFactory     (void) noexcept {}

public:
    virtual                         ~GpTypeMapperFactory    (void) noexcept {}

    virtual GpTypeMapper::SP        NewInstance             (void) const = 0;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
