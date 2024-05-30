#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include "../GpReflectObject.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpExceptionDesc: public GpReflectObject
{
public:
    CLASS_DD(GpExceptionDesc)
    REFLECT_DECLARE("eb884688-11b9-4412-9c70-7e48c3231f5f"_uuid)

public:
                        GpExceptionDesc         (void) noexcept;
    explicit            GpExceptionDesc         (const GpExceptionDesc& aDesc);
    explicit            GpExceptionDesc         (GpExceptionDesc&& aDesc) noexcept;
    virtual             ~GpExceptionDesc        (void) noexcept override;

    void                SetFromExceptionSTD     (const std::exception&  aException,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current());
    void                SetFromExceptionGP      (const GpException& aException);

public:
    std::string         message;
    u_int_32            line;
    u_int_32            column;
    std::string         file_name;
    std::string         function_name;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
