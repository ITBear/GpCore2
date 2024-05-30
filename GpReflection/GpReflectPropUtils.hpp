#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectUtils.hpp"
#include "GpReflectProp.hpp"

namespace GPlatform {

class GpReflectObject;

class GP_REFLECTION_API GpReflectPropUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectPropUtils)

    using ObjectPropsBindVecT = std::vector<std::tuple<const void*, const GpReflectProp&>>;

public:
    static GpReflectProp::C::Vec::CRef  SPropsFlagFilter    (const GpReflectProp::SmallVecVal&  aProps,
                                                             const GpReflectPropFlag::EnumT     aFlag);
    template<typename T> static
    GpReflectProp&                      SAddProp            (std::string&&                      aPropName,
                                                             const std::ptrdiff_t               aOffset,
                                                             const GpReflectPropFlags           aFlags,
                                                             GpReflectProp::FlagArgsT&&         aFlagArgs,
                                                             GpReflectProp::GenFnOptT           aGenFn,
                                                             GpReflectProp::SmallVecVal&        aPropsOut,
                                                             GpReflectProp::FromStringFnMapT    aFromStringFns);
};

template<typename T>
GpReflectProp&  GpReflectPropUtils::SAddProp
(
    std::string&&                   aPropName,
    const std::ptrdiff_t            aOffset,
    const GpReflectPropFlags        aFlags,
    GpReflectProp::FlagArgsT&&      aFlagArgs,
    GpReflectProp::GenFnOptT        aGenFn,
    GpReflectProp::SmallVecVal&     aPropsOut,
    GpReflectProp::FromStringFnMapT aFromStringFns
)
{
    constexpr const auto    types           = GpReflectUtils::SDetectTypes<T>();
    constexpr const auto    valueType       = std::get<0>(types);
    constexpr const auto    keyType         = std::get<1>(types);
    constexpr const auto    containerType   = std::get<2>(types);

    constexpr const GpUUID  modelUid        = GpReflectUtils::SDetectModelUid<T>();
    constexpr const size_t  align           = alignof(T);
    constexpr const size_t  size            = sizeof(T);

    GpReflectProp::ProcessCustomFnOptT  constructCustomFn;
    GpReflectProp::ProcessCustomFnOptT  destructCustomFn;

    if constexpr (   (   (valueType == GpReflectType::ENUM)
                      || (valueType == GpReflectType::ENUM_FLAGS))
                  && (keyType == GpReflectType::NOT_SET)
                  && (containerType == GpReflectContainerType::NO))
    {
        constructCustomFn = [](void* aPropPtr)
        {
            MemOps::SConstruct<T>
            (
                static_cast<T*>(aPropPtr),
                1
            );
        };

        destructCustomFn = [](void* aPropPtr)
        {
            MemOps::SDestruct<T>
            (
                static_cast<T*>(aPropPtr),
                1
            );
        };
    }

    const size_t propIdx = std::size(aPropsOut);

    aPropsOut.emplace_back
    (
        GpReflectProp
        (
            valueType,
            modelUid,
            containerType,
            keyType,
            std::move(aPropName),
            align,
            size,
            aOffset,
            aFlags,
            std::move(aFlagArgs),
            aGenFn,
            constructCustomFn,
            destructCustomFn,
            std::move(aFromStringFns),
            propIdx
        )
    );

    return aPropsOut.at(std::size(aPropsOut) - 1);
}

}// namespace GPlatform
