#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectProp.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

void    GpReflectUtils::SGenerateOnce
(
    const GpReflectModel&           aModel,
    GpReflectObject::C::Vec::SP&    aItems
)
{
    const GpReflectProp::C::Vec::CRef genOnceProps = GpReflectPropUtils::SPropsFlagFilter(aModel.Props(), GpReflectPropFlag::GENERATED_ONCE);

    for (GpReflectObject::SP& itemSP: aItems)
    {
        GpReflectObject&    item        = itemSP.V();
        void*               itemDataPtr = item.ReflectDataPtr();

        for (const GpReflectProp& prop: genOnceProps)
        {
            prop.GenFn(itemDataPtr);
        }
    }
}

void    GpReflectUtils::SGenerateOnce (GpReflectObject& aItem)
{
    GpReflectModel::CSP                 modelCSP        = aItem.ReflectModel();
    const GpReflectProp::C::Vec::CRef   genOnceProps    = GpReflectPropUtils::SPropsFlagFilter
    (
        modelCSP.Vn().Props(),
        GpReflectPropFlag::GENERATED_ONCE
    );

    void* itemDataPtr = aItem.ReflectDataPtr();

    for (const GpReflectProp& prop: genOnceProps)
    {
        prop.GenFn(itemDataPtr);
    }
}

}// namespace GPlatform
