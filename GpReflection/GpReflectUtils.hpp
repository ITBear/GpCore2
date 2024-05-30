#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflection_global.hpp"
#include "GpReflectType.hpp"
#include "GpReflectContainerType.hpp"
#include "GpReflectObjWrapVector.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>

namespace GPlatform {

class GpReflectObject;
class GpReflectModel;

TAG_REGISTER(GpReflectObject)

class GP_REFLECTION_API GpReflectUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils)

public:
    template<GpReflectType::EnumT T>
    struct  Decltype
    {
    };

    template<>
    struct  Decltype<GpReflectType::NOT_SET>
    {
    };

    template<>
    struct  Decltype<GpReflectType::U_INT_8>
    {
        using type_t = u_int_8;
    };

    template<>
    struct  Decltype<GpReflectType::S_INT_8>
    {
        using type_t = s_int_8;
    };

    template<>
    struct  Decltype<GpReflectType::U_INT_16>
    {
        using type_t = u_int_16;
    };

    template<>
    struct  Decltype<GpReflectType::S_INT_16>
    {
        using type_t = s_int_16;
    };

    template<>
    struct  Decltype<GpReflectType::U_INT_32>
    {
        using type_t = u_int_32;
    };

    template<>
    struct  Decltype<GpReflectType::S_INT_32>
    {
        using type_t = s_int_32;
    };

    template<>
    struct  Decltype<GpReflectType::U_INT_64>
    {
        using type_t = u_int_64;
    };

    template<>
    struct  Decltype<GpReflectType::S_INT_64>
    {
        using type_t = s_int_64;
    };

    template<>
    struct  Decltype<GpReflectType::DOUBLE>
    {
        using type_t = double;
    };

    template<>
    struct  Decltype<GpReflectType::FLOAT>
    {
        using type_t = float;
    };

    template<>
    struct  Decltype<GpReflectType::BOOLEAN>
    {
        using type_t = bool;
    };

    template<>
    struct  Decltype<GpReflectType::UUID>
    {
        using type_t = GpUUID;
    };

    template<>
    struct  Decltype<GpReflectType::STRING>
    {
        using type_t = std::string;
    };

    template<>
    struct  Decltype<GpReflectType::BLOB>
    {
        using type_t = GpBytesArray;
    };

    template<GpReflectType::EnumT T>
    using DecltypeT = typename Decltype<T>::type_t;

    template<typename T> [[nodiscard]] static
    constexpr std::string_view      SModelName                  (void) noexcept;

    template<typename T, typename ValT> [[nodiscard]] static
    constexpr std::ptrdiff_t        SOffsetOf                   (ValT T::*element);

    template<typename T> [[nodiscard]] static
    constexpr GpReflectType::EnumT  SDetectType                 (void);

    template<typename T> [[nodiscard]] static
    constexpr std::tuple<GpReflectType::EnumT/*value type*/, GpReflectType::EnumT/*map key type*/, GpReflectContainerType::EnumT/*container type*/>
                                    SDetectTypes                (void);

    template<typename T, GpReflectType::EnumT Type> [[nodiscard]] static
    constexpr GpUUID                SGetModelUid                (void);

    template<typename T> [[nodiscard]] static
    constexpr GpUUID                SDetectModelUid             (void);

    inline static constexpr bool    SCheckContainerValueType    (const GpReflectType::EnumT aType);
    inline static constexpr bool    SCheckVectorWrapValueType   (const GpReflectType::EnumT aType);
    inline static constexpr bool    SCheckContainerKeyType      (const GpReflectType::EnumT aType);

    template<typename T> [[nodiscard]] static
    T                               SCopyValue                  (const T& aValue);

    static void                     SGenerateOnce               (const GpReflectModel&                  aModel,
                                                                 std::vector<GpSP<GpReflectObject>>&    aItems);
    static void                     SGenerateOnce               (GpReflectObject&                       aItem);
};

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
#elif defined(GP_COMPILER_MSVC)
    constexpr std::string_view  pname       = __FUNCSIG__;
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
    } else if constexpr (std::is_same_v<VT, std::string>)
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
            GpThrowCe<GpException>("Unknown type");
        }
    } else if constexpr (GpHasTag_GpUnit<VT>())
    {
        constexpr GpReflectType::EnumT t = GpReflectUtils::SDetectType<typename VT::value_type>();

        if constexpr (t == GpReflectType::NOT_SET)
        {
            GpThrowCe<GpException>("Unknown type");
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
constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> GpReflectUtils::SDetectTypes (void)
{
    constexpr GpReflectType::EnumT t = SDetectType<T>();

    if constexpr (t != GpReflectType::NOT_SET)
    {
        return {t, GpReflectType::NOT_SET, GpReflectContainerType::NO};
    } else
    {
        if constexpr (std::is_same_v<T, GpVectorReflectObjWrap<typename T::value_type>>)
        {
            constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> res
                = {SDetectType<typename T::value_type>(), GpReflectType::NOT_SET, GpReflectContainerType::VECTOR_WRAP};

            if constexpr (!SCheckVectorWrapValueType(std::get<0>(res)))
            {
                GpThrowCe<GpException>("Unsupported type '"_sv + SModelName<T>() + "' for vector wrap value");
            }

            return res;
        } else if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
        {
            constexpr std::tuple<GpReflectType::EnumT, GpReflectType::EnumT, GpReflectContainerType::EnumT> res
                = {SDetectType<typename T::value_type>(), GpReflectType::NOT_SET, GpReflectContainerType::VECTOR};

            if constexpr (!SCheckContainerValueType(std::get<0>(res)))
            {
                GpThrowCe<GpException>("Unsupported type '"_sv + SModelName<T>() + "' for vector value");
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
                GpThrowCe<GpException>("Unsupported type '"_sv + SModelName<T>() + "' for map key");
            }

            if constexpr (!SCheckContainerValueType(std::get<0>(res)))
            {
                GpThrowCe<GpException>("Unsupported type '"_sv + SModelName<T>() + "' for map mapped_type");
            }

            return res;
        } else
        {
            GpThrowCe<GpException>("Unknown type '"_sv + SModelName<T>() + "'");
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
        if constexpr (std::is_same_v<T, GpVectorReflectObjWrap<typename T::value_type>>)
        {
            constexpr GpReflectType::EnumT et = SDetectType<typename T::value_type>();
            return SGetModelUid<typename T::value_type, et>();
        } else if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
        {
            constexpr GpReflectType::EnumT et = SDetectType<typename T::value_type>();
            return SGetModelUid<typename T::value_type, et>();
        } else if constexpr (std::is_same_v<T, std::map<typename T::key_type, typename T::mapped_type, std::less<>>>)
        {
            constexpr GpReflectType::EnumT et = SDetectType<typename T::mapped_type>();
            return SGetModelUid<typename T::mapped_type, et>();
        } else
        {
            GpThrowCe<GpException>("Unknown model '"_sv + SModelName<T>() + "'");
        }
    }

    return GpUUID();
}

constexpr bool  GpReflectUtils::SCheckContainerValueType (const GpReflectType::EnumT aType)
{
    return (aType != GpReflectType::ENUM)
        && (aType != GpReflectType::ENUM_FLAGS)
        && (aType != GpReflectType::BOOLEAN)
        && (aType != GpReflectType::NOT_SET);
}

constexpr bool  GpReflectUtils::SCheckVectorWrapValueType (const GpReflectType::EnumT aType)
{
    return (aType == GpReflectType::OBJECT)
        && (aType != GpReflectType::NOT_SET);
}

constexpr bool  GpReflectUtils::SCheckContainerKeyType (const GpReflectType::EnumT aType)
{
    return (aType != GpReflectType::BOOLEAN)
        && (aType != GpReflectType::OBJECT)
        && (aType != GpReflectType::OBJECT_SP)
        && (aType != GpReflectType::ENUM)
        && (aType != GpReflectType::ENUM_FLAGS)
        && (aType != GpReflectType::NOT_SET);
}

template<typename T> [[nodiscard]]
T   GpReflectUtils::SCopyValue (const T& aValue)
{
    using VT = std::remove_cvref_t<T>;

         if constexpr (std::is_arithmetic_v<VT>) return aValue;
    else if constexpr (std::is_same_v<VT, GpUUID>) return aValue;
    else if constexpr (std::is_same_v<VT, std::string>) return aValue;
    else if constexpr (std::is_same_v<VT, GpBytesArray>) return aValue;
    else if constexpr (std::is_base_of_v<GpReflectObject, VT>) return aValue;
    else if constexpr (GpHasTag_GpSharedPtrBase<VT>())
    {
        if constexpr (GpHasTag_GpReflectObject<typename VT::value_type>())
        {
            if (aValue.IsNotNULL())
            {
                return aValue.Vn().template ReflectClone<typename VT::value_type>();
            } else
            {
                return {};
            }
        } else
        {
            GpThrowCe<GpException>("Unknown type");
        }
    } else if constexpr (GpHasTag_GpUnit<VT>()) return aValue;
    else if constexpr (GpHasTag_GpEnum<VT>()) return aValue;
    else if constexpr (GpHasTag_GpEnumFlags<VT>()) return aValue;
    else if constexpr (std::is_same_v<VT, GpVectorReflectObjWrap<typename VT::value_type>>)
    {
        if constexpr (GpHasTag_GpReflectObject<typename VT::value_type>())
        {
            return aValue;
        } else
        {
            GpThrowCe<GpException>("Unknown type");
        }
    } else if constexpr (std::is_same_v<VT, std::vector<typename VT::value_type>>)
    {
        if constexpr (GpHasTag_GpSharedPtrBase<typename VT::value_type>())
        {
            if constexpr (GpHasTag_GpReflectObject<typename VT::value_type::value_type>())
            {
                T tmp;
                tmp.reserve(std::size(aValue));

                for (const auto& e: aValue)
                {
                    if (e.IsNotNULL())
                    {
                        GpSP<GpReflectObject> val = e.Vn().ReflectClone();
                        tmp.emplace_back(val.CastUpAs<typename T::value_type>());
                    } else
                    {
                        tmp.emplace_back(typename T::value_type{});
                    }
                }

                return tmp;
            } else
            {
                GpThrowCe<GpException>("Unknown type");
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
                    if (value.IsNotNULL())
                    {
                        GpSP<GpReflectObject> val = value.V().ReflectClone();
                        tmp.insert_or_assign(key, val.CastUpAs<typename T::mapped_type>());
                    } else
                    {
                        tmp.insert_or_assign(key, typename T::mapped_type{});
                    }
                }

                return tmp;
            } else
            {
                GpThrowCe<GpException>("Unknown type");
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

}// namespace GPlatform
