#include "GpTypeStructBase.hpp"
#include "GpTypeManager.hpp"
#include "GpTypeUtils.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GP_TYPE_STRUCT_STATIC_TYPE_REG_IMPL(GpTypeStructBase);

GpTypeStructBase::SP    GpTypeStructBase::StructFactory::NewInstance (void) const
{
    return MakeSP<GpTypeStructBase>();
}

const GpTypeStructInfo& GpTypeStructBase::STypeInfo (void)
{
    static const GpTypeStructInfo sStructInfo = GpTypeStructBase::_SCollectStructInfo();
    return sStructInfo;
}

size_t  GpTypeStructBase::SStaticTypeReg (void)
{
    return _sStaticTypeReg;
}

const GpTypeStructInfo& GpTypeStructBase::_TypeInfo (void) const noexcept
{
    return GpTypeStructBase::STypeInfo();
}

GpTypeStructBase::SP    GpTypeStructBase::_NewInstance (void) const
{
    return MakeSP<GpTypeStructBase>();
}

GpTypeStructInfo    GpTypeStructBase::_SCollectStructInfo (void)
{
    GpTypePropInfo::C::Vec::Val props;
    GpTypeStructFactory::SP     factory     = GpSP<StructFactory>::SNew();
    constexpr GpUUID            typeUID     = GpTypeStructBase::STypeUID();
    constexpr GpUUID            groupUID    = GpUUID::CE_FromString("96155f96-6bc0-434e-a2da-0f9e72368461"_sv);
    GpTypeStructInfo structInfo
    (
        typeUID,
        GpUUID(),
        std::string(GpTypeUtils::STypeName<GpTypeStructBase>()),
        std::move(props),
        groupUID,
        factory
    );

    return structInfo;
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
