#include "GpReflectUtils.hpp"

namespace GPlatform {

void    GpReflectUtils::SGenerateOnce
(
    const GpReflectModel&           aModel,
    GpReflectObject::C::Vec::SP&    aItems
)
{
    const GpReflectProp::C::Vec::CRef genOnceProps = GpReflectUtils::SPropsFlagFilter(aModel.Props(), GpReflectPropFlag::GENERATED_ONCE);

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
    const GpReflectModel&               model           = aItem.ReflectModel();
    const GpReflectProp::C::Vec::CRef   genOnceProps    = GpReflectUtils::SPropsFlagFilter
    (
        model.Props(),
        GpReflectPropFlag::GENERATED_ONCE
    );

    void* itemDataPtr = aItem.ReflectDataPtr();

    for (const GpReflectProp& prop: genOnceProps)
    {
        prop.GenFn(itemDataPtr);
    }
}

}//namespace GPlatform
