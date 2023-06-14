#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpUtils/Types/Strings/GpUTF.hpp"
#include "GpReflectObject.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils)

public:
    [[nodiscard]] inline static
    GpReflectProp::C::Vec::CRef     SPropsFlagFilter        (const GpReflectProp::C::Vec::Val&  aProps,
                                                             const GpReflectPropFlag::EnumT     aFlag);

    template<typename T> [[nodiscard]] static
    constexpr std::string_view      SModelName              (void) noexcept;

    template<typename T, typename ValT> [[nodiscard]] static
    constexpr std::ptrdiff_t        SOffsetOf               (ValT T::*element);

    template<typename T> [[nodiscard]] static
    constexpr GpReflectType::EnumT  SDetectType             (void);

    template<typename T> [[nodiscard]] static
    constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT>
                                    SDetectContainerType    (void);

    template<typename T, GpReflectType::EnumT Type> [[nodiscard]] static
    constexpr GpUUID                SGetModelUid            (void);

    template<typename T> [[nodiscard]] static
    constexpr GpUUID                SDetectModelUid         (void);

    template<typename T> static
    GpReflectProp&                  SAddProp                (std::u8string&&                    aPropName,
                                                             const std::ptrdiff_t               aOffset,
                                                             const GpReflectPropFlags           aFlags,
                                                             GpReflectProp::FlagArgsT&&         aFlagArgs,
                                                             GpReflectProp::GenFnOptT           aGenFn,
                                                             GpReflectProp::C::Vec::Val&        aPropsOut,
                                                             GpReflectProp::FromStringFnMapT    aFromStringFns);
    template<typename T> static
    void                            SReflectModelInit       (void);

    inline static constexpr bool    SCheckContainerValueType(const GpReflectType::EnumT aType);
    inline static constexpr bool    SCheckContainerKeyType  (const GpReflectType::EnumT aType);

    template<typename T> [[nodiscard]] static
    T                               SCopyValue              (const T& aValue);

    static void                     SGenerateOnce           (const GpReflectModel&          aModel,
                                                             GpReflectObject::C::Vec::SP&   aItems);
    static void                     SGenerateOnce           (GpReflectObject&               aItem);
};

GpReflectProp::C::Vec::CRef GpReflectUtils::SPropsFlagFilter
(
    const GpReflectProp::C::Vec::Val&   aProps,
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

template<typename T> [[nodiscard]]
constexpr std::string_view  GpReflectUtils::SModelName (void) noexcept
{
#if defined(GP_COMPILER_GCC)
    constexpr std::string_view  pname       = __PRETTY_FUNCTION__;
    constexpr size_t            part_1_id   = pname.find_first_of('=');
    constexpr std::string_view  part_1      = pname.substr(part_1_id+2);
    constexpr size_t            part_2_id   = part_1.find_first_of(';');
    constexpr std::string_view  part_2      = part_1.substr(0, part_2_id);

    return part_2;
#elif defined(GP_COMPILER_CLANG)
    constexpr std::string_view  pname       = __PRETTY_FUNCTION__;
    constexpr size_t            part_1_id   = pname.find_first_of('=');
    constexpr std::string_view  part_1      = pname.substr(part_1_id+2);
    constexpr size_t            part_2_id   = part_1.find_first_of(']');
    constexpr std::string_view  part_2      = part_1.substr(0, part_2_id);

    return part_2;
#else
#   error Unsupported compiler
#endif
}

template<typename T, typename ValT> [[nodiscard]]
constexpr std::ptrdiff_t    GpReflectUtils::SOffsetOf(ValT T::*element)
{
    return std::ptrdiff_t(&(static_cast<T*>(nullptr)->*element));
}

template<typename T> [[nodiscard]]
constexpr GpReflectType::EnumT  GpReflectUtils::SDetectType (void)
{
    using VT = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<VT, u_int_8>)
    {
        return GpReflectType::U_INT_8;
    } else if constexpr (std::is_same_v<VT, s_int_8>)
    {
        return GpReflectType::S_INT_8;
    } else if constexpr (std::is_same_v<VT, u_int_16>)
    {
        return GpReflectType::U_INT_16;
    } else if constexpr (std::is_same_v<VT, s_int_16>)
    {
        return GpReflectType::S_INT_16;
    } else if constexpr (std::is_same_v<VT, u_int_32>)
    {
        return GpReflectType::U_INT_32;
    } else if constexpr (std::is_same_v<VT, s_int_32>)
    {
        return GpReflectType::S_INT_32;
    } else if constexpr (std::is_same_v<VT, u_int_64>)
    {
        return GpReflectType::U_INT_64;
    } else if constexpr (std::is_same_v<VT, s_int_64>)
    {
        return GpReflectType::S_INT_64;
    } else if constexpr (std::is_same_v<VT, double>)
    {
        return GpReflectType::DOUBLE;
    } else if constexpr (std::is_same_v<VT, float>)
    {
        return GpReflectType::FLOAT;
    } else if constexpr (std::is_same_v<VT, bool>)
    {
        return GpReflectType::BOOLEAN;
    } else if constexpr (std::is_same_v<VT, GpUUID>)
    {
        return GpReflectType::UUID;
    } else if constexpr (std::is_same_v<VT, std::u8string>)
    {
        return GpReflectType::STRING;
    } else if constexpr (std::is_same_v<VT, GpBytesArray>)
    {
        return GpReflectType::BLOB;
    } else if constexpr (std::is_base_of_v<GpReflectObject, VT>)
    {
        return GpReflectType::OBJECT;
    } else if constexpr (GpHasTag_GpSharedPtrBase<VT>())
    {
        if constexpr (GpHasTag_GpReflectObject<typename VT::value_type>())
        {
            return GpReflectType::OBJECT_SP;
        } else
        {
            GpThrowCe<GpException>(u8"Unknown type");
        }
    } else if constexpr (GpHasTag_GpUnit<VT>())
    {
        constexpr GpReflectType::EnumT t = GpReflectUtils::SDetectType<typename VT::value_type>();

        if constexpr (t == GpReflectType::NOT_SET)
        {
            GpThrowCe<GpException>(u8"Unknown type");
        }

        return t;
    } else if constexpr (GpHasTag_GpEnum<VT>())
    {
        return GpReflectType::ENUM;
    } else if constexpr (GpHasTag_GpEnumFlags<VT>())
    {
        return GpReflectType::ENUM_FLAGS;
    } else
    {
        return GpReflectType::NOT_SET;
    }
}

template<typename T> [[nodiscard]]
constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> GpReflectUtils::SDetectContainerType (void)
{
    constexpr GpReflectType::EnumT t = SDetectType<T>();

    if constexpr (t != GpReflectType::NOT_SET)
    {
        return {t, GpReflectType::NOT_SET, GpReflectContainerType::NO};
    } else
    {
        if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
        {
            constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> res
                = {SDetectType<typename T::value_type>(), GpReflectType::NOT_SET, GpReflectContainerType::VECTOR};

            if constexpr (!SCheckContainerValueType(std::get<0>(res)))
            {
                GpThrowCe<GpException>(u8"Unsupported type '"_sv + SModelName<T>() + u8"' for vector value");
            }

            return res;
        } else if constexpr (std::is_same_v<T, std::map<typename T::key_type, typename T::mapped_type, std::less<>>>)
        {
            constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> res
                = {SDetectType<typename T::mapped_type>(),
                   SDetectType<typename T::key_type>(),
                   GpReflectContainerType::MAP};

            if constexpr (!SCheckContainerKeyType(std::get<1>(res)))
            {
                GpThrowCe<GpException>(u8"Unsupported type '"_sv + SModelName<T>() + u8"' for map key");
            }

            if constexpr (!SCheckContainerValueType(std::get<0>(res)))
            {
                GpThrowCe<GpException>(u8"Unsupported type '"_sv + SModelName<T>() + u8"' for map mapped_type");
            }

            return res;
        } else
        {
            GpThrowCe<GpException>(u8"Unknown type '"_sv + SModelName<T>() + u8"'");
        }
    }

    return {GpReflectType::NOT_SET, GpReflectType::NOT_SET, GpReflectContainerType::NO};
}

template<typename T, GpReflectType::EnumT Type> [[nodiscard]]
constexpr  GpUUID   GpReflectUtils::SGetModelUid (void)
{
    if constexpr (Type == GpReflectType::OBJECT)
    {
        return T::SReflectModelUid();
    } else if constexpr (Type == GpReflectType::OBJECT_SP)
    {
        return T::value_type::SReflectModelUid();
    } else
    {
        return GpUUID();
    }
}

template<typename T> [[nodiscard]]
constexpr GpUUID    GpReflectUtils::SDetectModelUid (void)
{
    constexpr GpReflectType::EnumT t = SDetectType<T>();

    if constexpr (t != GpReflectType::NOT_SET)
    {
        return SGetModelUid<T, t>();
    } else
    {
        if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
        {
            constexpr GpReflectType::EnumT et = SDetectType<typename T::value_type>();
            return SGetModelUid<typename T::value_type, et>();
        } else if constexpr (std::is_same_v<T, std::map<typename T::key_type, typename T::mapped_type, std::less<>>>)
        {
            constexpr GpReflectType::EnumT et = SDetectType<typename T::mapped_type>();
            return SGetModelUid<typename T::mapped_type, et>();
        } else
        {
            GpThrowCe<GpException>(u8"Unknown model '"_sv + SModelName<T>() + u8"'");
        }
    }

    return GpUUID();
}

template<typename T>
GpReflectProp&  GpReflectUtils::SAddProp
(
    std::u8string&&                 aPropName,
    const std::ptrdiff_t            aOffset,
    const GpReflectPropFlags        aFlags,
    GpReflectProp::FlagArgsT&&      aFlagArgs,
    GpReflectProp::GenFnOptT        aGenFn,
    GpReflectProp::C::Vec::Val&     aPropsOut,
    GpReflectProp::FromStringFnMapT aFromStringFns
)
{
    constexpr const auto        types   = GpReflectUtils::SDetectContainerType<T>();
    constexpr const GpUUID      modelUid= GpReflectUtils::SDetectModelUid<T>();
    constexpr const size_t      align   = alignof(T);
    constexpr const size_t      size    = sizeof(T);

    GpReflectProp::ProcessCustomFnOptT  constructCustomFn;
    GpReflectProp::ProcessCustomFnOptT  destructCustomFn;

    if constexpr (   (   (std::get<0>(types) == GpReflectType::ENUM)
                      || (std::get<0>(types) == GpReflectType::ENUM_FLAGS))
                  && (std::get<1>(types) == GpReflectType::NOT_SET)
                  && (std::get<2>(types) == GpReflectContainerType::NO))
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

    aPropsOut.emplace_back
    (
        GpReflectProp
        (
            std::get<0>(types),
            modelUid,
            std::get<2>(types),
            std::get<1>(types),
            std::move(aPropName),
            align,
            size,
            aOffset,
            aFlags,
            std::move(aFlagArgs),
            aGenFn,
            constructCustomFn,
            destructCustomFn,
            std::move(aFromStringFns)
        )
    );

    return aPropsOut.at(aPropsOut.size() - 1);
}

constexpr bool  GpReflectUtils::SCheckContainerValueType (const GpReflectType::EnumT aType)
{
    return (aType != GpReflectType::OBJECT)
        && (aType != GpReflectType::ENUM)
        && (aType != GpReflectType::ENUM_FLAGS);
}

constexpr bool  GpReflectUtils::SCheckContainerKeyType (const GpReflectType::EnumT aType)
{
    return (aType != GpReflectType::BOOLEAN)
        && (aType != GpReflectType::OBJECT)
        && (aType != GpReflectType::OBJECT_SP)
        && (aType != GpReflectType::ENUM)
        && (aType != GpReflectType::ENUM_FLAGS);
}

template<typename T> [[nodiscard]]
T   GpReflectUtils::SCopyValue (const T& aValue)
{
    using VT = std::remove_cvref_t<T>;

         if constexpr (std::is_arithmetic_v<VT>) return aValue;
    else if constexpr (std::is_same_v<VT, GpUUID>) return aValue;
    else if constexpr (std::is_same_v<VT, std::u8string>) return aValue;
    else if constexpr (std::is_same_v<VT, GpBytesArray>) return aValue;
    else if constexpr (std::is_base_of_v<GpReflectObject, VT>) return aValue;
    else if constexpr (GpHasTag_GpSharedPtrBase<VT>())
    {
        if constexpr (GpHasTag_GpReflectObject<typename VT::value_type>())
        {
            return aValue.V().template ReflectClone<typename VT::value_type>();
        } else
        {
            GpThrowCe<GpException>(u8"Unknown type");
        }
    } else if constexpr (GpHasTag_GpUnit<VT>()) return aValue;
    else if constexpr (GpHasTag_GpEnum<VT>()) return aValue;
    else if constexpr (GpHasTag_GpEnumFlags<VT>()) return aValue;
    else if constexpr (std::is_same_v<VT, std::vector<typename VT::value_type>>)
    {
        if constexpr (GpHasTag_GpSharedPtrBase<typename VT::value_type>())
        {
            if constexpr (GpHasTag_GpReflectObject<typename VT::value_type::value_type>())
            {
                T tmp;
                tmp.reserve(aValue.size());

                for (const auto& e: aValue)
                {
                    GpReflectObject::SP val = e.V().ReflectClone();
                    tmp.emplace_back(val.CastUpAs<typename T::value_type>());
                }

                return tmp;
            } else
            {
                GpThrowCe<GpException>(u8"Unknown type");
            }
        } else
        {
            return aValue;
        }
    } else if constexpr (std::is_same_v<VT, std::map<typename VT::key_type, typename VT::mapped_type, std::less<>>>)
    {
        if constexpr (GpHasTag_GpSharedPtrBase<typename VT::mapped_type>())
        {
            if constexpr (GpHasTag_GpReflectObject<typename VT::mapped_type::value_type>())
            {
                T tmp;

                for (const auto& [key, value]: aValue)
                {
                    GpReflectObject::SP val = value.V().ReflectClone();
                    tmp.try_emplace(key, val.CastUpAs<typename T::mapped_type>());
                }

                return tmp;
            } else
            {
                GpThrowCe<GpException>(u8"Unknown type");
            }
        } else
        {
            return aValue;
        }
    } else
    {
        return std::nullopt;
    }
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
