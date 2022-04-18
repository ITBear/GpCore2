#include "GpTypeStructDynamic.hpp"
#include "GpTypeStructDynamicUtils.hpp"

namespace GPlatform {

GpTypeStructBase::SP    GpTypeStructDynamic::Factory::NewInstance (const GpTypeStructInfo& aStructInfo) const
{
    return GpTypeStructDynamicUtils::SConstruct(aStructInfo);
}

void    GpTypeStructDynamic::Factory::ConstructStruct (void* /*aStructDataPtr*/) const
{
    THROW_GPE("Use sharep pointer to GpTypeStructDynamic"_sv);
}

void    GpTypeStructDynamic::Factory::DestructStruct (void* /*aStructDataPtr*/) const
{
    THROW_GPE("Use sharep pointer to GpTypeStructDynamic"_sv);
}

GpTypeStructDynamic::GpTypeStructDynamic
(
    const GpTypeStructInfo& aTypeInfo,
    void*                   aData
) noexcept:
iTypeInfo(aTypeInfo),
iData(aData)
{
}

GpTypeStructDynamic::~GpTypeStructDynamic (void) noexcept
{
    GpTypeStructDynamicUtils::SDestroy(iTypeInfo, iData);
    iData = nullptr;
}

void    GpTypeStructDynamic::_type_id_tag_fn (_type_id_tag_t&) const noexcept
{
    //NOP
}

const GpTypeStructInfo& GpTypeStructDynamic::_TypeInfo (void) const noexcept
{
    return iTypeInfo;
}

GpTypeStructBase::SP    GpTypeStructDynamic::_NewInstance (void) const
{
    //TODO: implement
    THROW_GPE_NOT_IMPLEMENTED();
}

GpTypeStructBase::SP    GpTypeStructDynamic::_Clone (void) const
{
    //TODO: implement
    THROW_GPE_NOT_IMPLEMENTED();
}

const void* GpTypeStructDynamic::_DataPtr (void) const noexcept
{
    return iData;
}

void*   GpTypeStructDynamic::_DataPtr (void) noexcept
{
    return iData;
}

}//namespace GPlatform
