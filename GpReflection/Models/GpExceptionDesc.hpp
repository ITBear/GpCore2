#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)
#if defined(GP_USE_REFLECTION)

#include "../GpReflectObject.hpp"
#include "../GpReflectUtils.hpp"
#include "../GpReflectManager.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpExceptionDesc: public GpReflectObject
{
public:
    CLASS_DD(GpExceptionDesc)
    REFLECT_DECLARE(u8"eb884688-11b9-4412-9c70-7e48c3231f5f"_uuid)

public:
                            GpExceptionDesc         (void) noexcept;
    explicit                GpExceptionDesc         (const GpExceptionDesc& aDesc);
    explicit                GpExceptionDesc         (GpExceptionDesc&& aDesc) noexcept;
    virtual                 ~GpExceptionDesc        (void) noexcept override;

    void                    SetFromExceptionSTD     (const std::exception&  aException,
                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current());
    void                    SetFromExceptionGP      (const GpException& aException);

public:
    std::u8string           message;
    u_int_32                line;
    u_int_32                column;
    std::u8string           file_name;
    std::u8string           function_name;
};

}//GPlatform

#endif//#if defined(GP_USE_REFLECTION)
#endif//#if defined(GP_USE_EXCEPTIONS)
