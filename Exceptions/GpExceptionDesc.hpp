#pragma once

#include "../GpCore_global.hpp"
#include "../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)
#if defined(GP_USE_TYPE_SYSTEM)

#include "GpException.hpp"
#include "../Types/TypeSystem/GpTypeSystem.hpp"

namespace GPlatform {

class GPCORE_API GpExceptionDesc: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpExceptionDesc)
    TYPE_STRUCT_DECLARE("eb884688-11b9-4412-9c70-7e48c3231f5f"_sv)

public:
                            GpExceptionDesc         (void) noexcept;
    virtual                 ~GpExceptionDesc        (void) noexcept override;

    void                    SetFromExceptionSTD     (const std::exception&  aException,
                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current());
    void                    SetFromExceptionGP      (const GpException& aException);

public:
    std::string             message;
    count_t                 line;
    count_t                 column;
    std::string             file_name;
    std::string             function_name;
};

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
#endif//#if defined(GP_USE_EXCEPTIONS)
