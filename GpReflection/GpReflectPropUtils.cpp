#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

GpReflectProp::C::Vec::CRef GpReflectPropUtils::SPropsFlagFilter
(
    const GpReflectProp::SmallVecVal&   aProps,
    const GpReflectPropFlag::EnumT      aFlag
)
{
    GpReflectProp::C::Vec::CRef res;

    for (const GpReflectProp& prop: aProps)
    {
        if (prop.FlagTest(aFlag))
        {
            res.emplace_back(prop);
        }
    }

    return res;
}

}// namespace GPlatform
