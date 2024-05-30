#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectPropDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpReflectPropDesc)
    REFLECT_DECLARE("f7a8691c-070a-4576-9e00-f4156b61758a"_uuid)

public:
                            GpReflectPropDesc   (void) noexcept;
                            GpReflectPropDesc   (const GpReflectPropDesc& aDesc);
                            GpReflectPropDesc   (GpReflectPropDesc&& aDesc) noexcept;
                            GpReflectPropDesc   (const GpReflectType::EnumT             aValType,
                                                 const GpReflectType::EnumT             aKeyType,
                                                 const GpReflectContainerType::EnumT    aContainerType,
                                                 const GpUUID&                          aModelUid,
                                                 std::string                            aName) noexcept;
    virtual                 ~GpReflectPropDesc  (void) noexcept override final;

public:
    GpReflectType           val_type;
    GpReflectType           key_type;
    GpReflectContainerType  container_type;
    GpUUID                  model_uid;
    std::string             name;
};

}// namespace GPlatform
