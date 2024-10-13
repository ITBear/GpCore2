#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectModel.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

#include <memory>

namespace GPlatform {

class GpReflectModelBuilder;

class GP_REFLECTION_API GpReflectModelPropBuilder
{
public:
    inline                              GpReflectModelPropBuilder   (void) noexcept;
    inline                              GpReflectModelPropBuilder   (GpReflectModelPropBuilder&& aBuilder) noexcept;
    inline                              GpReflectModelPropBuilder   (GpReflectModelBuilder& aBuilder) noexcept;
                                        ~GpReflectModelPropBuilder  (void) noexcept;

    inline GpReflectModelPropBuilder&   operator=                   (GpReflectModelPropBuilder&& aBuilder) noexcept;
    inline GpReflectModelPropBuilder&   Set                         (GpReflectModelPropBuilder&& aBuilder) noexcept;
    inline GpReflectModelPropBuilder&   SetModelBuilder             (GpReflectModelBuilder* aModelBuilder) noexcept;

    size_t                              MaxAlign                    (void) const noexcept {return iMaxAlign;}
    size_t                              TotalSize                   (void) const noexcept {return iTotalSize;}

    GpReflectModelBuilder&              DoneBuildProps              (void);

    template<typename T>
    GpReflectModelPropBuilder&          Prop                        (std::string_view aName);

    GpReflectModelPropBuilder&          UI8                         (std::string_view aName);
    GpReflectModelPropBuilder&          SI8                         (std::string_view aName);
    GpReflectModelPropBuilder&          UI16                        (std::string_view aName);
    GpReflectModelPropBuilder&          SI16                        (std::string_view aName);
    GpReflectModelPropBuilder&          UI32                        (std::string_view aName);
    GpReflectModelPropBuilder&          SI32                        (std::string_view aName);
    GpReflectModelPropBuilder&          UI64                        (std::string_view aName);
    GpReflectModelPropBuilder&          SI64                        (std::string_view aName);
    GpReflectModelPropBuilder&          Double                      (std::string_view aName);
    GpReflectModelPropBuilder&          Float                       (std::string_view aName);
    GpReflectModelPropBuilder&          Bool                        (std::string_view aName);
    GpReflectModelPropBuilder&          UUID                        (std::string_view aName);
    GpReflectModelPropBuilder&          String                      (std::string_view aName);
    GpReflectModelPropBuilder&          BLOB                        (std::string_view aName);
    GpReflectModelPropBuilder&          Object                      (std::string_view       aName,
                                                                     const GpReflectModel&  aModel);
    GpReflectModelPropBuilder&          ObjectSP                    (std::string_view       aName,
                                                                     const GpReflectModel&  aModel);
    template<typename EnumT>
    GpReflectModelPropBuilder&          Enum                        (std::string_view aName);

    template<typename EnumFlagsT>
    GpReflectModelPropBuilder&          EnumFlags                   (std::string_view aName);

    // Vector
    GpReflectModelPropBuilder&          Vec_UI8                     (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_SI8                     (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_UI16                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_SI16                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_UI32                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_SI32                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_UI64                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_SI64                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_Double                  (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_Float                   (std::string_view aName);
    //GpReflectModelPropBuilder&        Vec_Bool                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_UUID                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_String                  (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_BLOB                    (std::string_view aName);
    GpReflectModelPropBuilder&          Vec_Object                  (std::string_view       aName,
                                                                     const GpReflectModel&  aModel);
    GpReflectModelPropBuilder&          Vec_ObjectSP                (std::string_view       aName,
                                                                     const GpReflectModel&  aModel);

    // Map
    GpReflectModelPropBuilder&          Map_UI8                     (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_SI8                     (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_UI16                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_SI16                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_UI32                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_SI32                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_UI64                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_SI64                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_Double                  (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_Float                   (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    //GpReflectModelPropBuilder&        Map_Bool                    (std::string_view       aName,
    //                                                               GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_UUID                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_String                  (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_BLOB                    (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType);
    GpReflectModelPropBuilder&          Map_ObjectSP                (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType,
                                                                     const GpReflectModel&  aModel);

    void                                Check                       (void) const;
    GpReflectProp::SmallVecVal          MoveProps                   (void) noexcept {return std::move(iProps);}

private:
    template<typename T>
    [[nodiscard]] static ptrdiff_t      SCalcOffset                 (ptrdiff_t  aCurrentOffset) noexcept;
    [[nodiscard]] static ptrdiff_t      SCalcOffset                 (ptrdiff_t  aCurrentOffset,
                                                                     size_t     aAlign,
                                                                     size_t     aSize) noexcept;

    template<typename                       T,
             GpReflectType::EnumT           TE,
             GpReflectContainerType::EnumT  TC>
    GpReflectModelPropBuilder&          _AddProp                    (std::string_view aName);

    template<typename                       T,
             GpReflectType::EnumT           TE,
             GpReflectContainerType::EnumT  TC>
    GpReflectModelPropBuilder&          _AddProp                    (std::string_view       aName,
                                                                     const GpReflectModel&  aModel);

    template<typename                       VT,
             GpReflectType::EnumT           VTE>
    GpReflectModelPropBuilder&          _AddPropMap                 (std::string_view       aName,
                                                                     GpReflectType::EnumT   aKeyType,
                                                                     const GpUUID&          aModelUid);

private:
    GpReflectModelBuilder*      iModelBuilder   = nullptr;
    GpReflectProp::SmallVecVal  iProps;
    std::ptrdiff_t              iOffset         = 0;
    size_t                      iMaxAlign       = 1;
    size_t                      iTotalSize      = 0;
};

GpReflectModelPropBuilder::GpReflectModelPropBuilder (void) noexcept
{
}

GpReflectModelPropBuilder::GpReflectModelPropBuilder (GpReflectModelPropBuilder&& aBuilder) noexcept:
iModelBuilder(aBuilder.iModelBuilder),
iProps       (std::move(aBuilder.iProps)),
iOffset      (std::move(aBuilder.iOffset))
{
}

GpReflectModelPropBuilder::GpReflectModelPropBuilder (GpReflectModelBuilder& aBuilder) noexcept:
iModelBuilder(&aBuilder)
{
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::operator= (GpReflectModelPropBuilder&& aBuilder) noexcept
{
    return Set(std::move(aBuilder));
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Set (GpReflectModelPropBuilder&& aBuilder) noexcept
{
    iModelBuilder   = aBuilder.iModelBuilder;
    iProps          = std::move(aBuilder.iProps);
    iOffset         = std::move(aBuilder.iOffset);

    return *this;
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::SetModelBuilder (GpReflectModelBuilder* aModelBuilder) noexcept
{
    iModelBuilder = aModelBuilder;
    return *this;
}

template<typename T>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Prop (std::string_view aName)
{
    constexpr const auto types          = GpReflectUtils::SDetectTypes<T>();
    constexpr const auto valueType      = std::get<0>(types);
    constexpr const auto keyType        = std::get<1>(types);
    constexpr const auto containerType  = std::get<2>(types);

    if constexpr(containerType == GpReflectContainerType::NO)
    {
             if constexpr(valueType == GpReflectType::U_INT_8)      return UI8(aName);
        else if constexpr(valueType == GpReflectType::S_INT_8)      return SI8(aName);
        else if constexpr(valueType == GpReflectType::U_INT_16)     return UI16(aName);
        else if constexpr(valueType == GpReflectType::S_INT_16)     return SI16(aName);
        else if constexpr(valueType == GpReflectType::U_INT_32)     return UI32(aName);
        else if constexpr(valueType == GpReflectType::S_INT_32)     return SI32(aName);
        else if constexpr(valueType == GpReflectType::U_INT_64)     return UI64(aName);
        else if constexpr(valueType == GpReflectType::S_INT_64)     return SI64(aName);
        else if constexpr(valueType == GpReflectType::DOUBLE)       return Double(aName);
        else if constexpr(valueType == GpReflectType::FLOAT)        return Float(aName);
        else if constexpr(valueType == GpReflectType::BOOLEAN)      return Bool(aName);
        else if constexpr(valueType == GpReflectType::UUID)         return UUID(aName);
        else if constexpr(valueType == GpReflectType::STRING)       return String(aName);
        else if constexpr(valueType == GpReflectType::BLOB)         return BLOB(aName);
        else if constexpr(valueType == GpReflectType::OBJECT)       return Object(aName, T::SReflectModel());
        else if constexpr(valueType == GpReflectType::OBJECT_SP)    return ObjectSP(aName, T::SReflectModel());
        else if constexpr(valueType == GpReflectType::ENUM)         return Enum<T>(aName);
        else if constexpr(valueType == GpReflectType::ENUM_FLAGS)   return EnumFlags<T>(aName);
        else
        {
            // if constexpr(valueType == GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    } else if constexpr(containerType == GpReflectContainerType::VECTOR)
    {
             if constexpr(valueType == GpReflectType::U_INT_8)      return Vec_UI8(aName);
        else if constexpr(valueType == GpReflectType::S_INT_8)      return Vec_SI8(aName);
        else if constexpr(valueType == GpReflectType::U_INT_16)     return Vec_UI16(aName);
        else if constexpr(valueType == GpReflectType::S_INT_16)     return Vec_SI16(aName);
        else if constexpr(valueType == GpReflectType::U_INT_32)     return Vec_UI32(aName);
        else if constexpr(valueType == GpReflectType::S_INT_32)     return Vec_SI32(aName);
        else if constexpr(valueType == GpReflectType::U_INT_64)     return Vec_UI64(aName);
        else if constexpr(valueType == GpReflectType::S_INT_64)     return Vec_SI64(aName);
        else if constexpr(valueType == GpReflectType::DOUBLE)       return Vec_Double(aName);
        else if constexpr(valueType == GpReflectType::FLOAT)        return Vec_Float(aName);
        else if constexpr(valueType == GpReflectType::UUID)         return Vec_UUID(aName);
        else if constexpr(valueType == GpReflectType::STRING)       return Vec_String(aName);
        else if constexpr(valueType == GpReflectType::BLOB)         return Vec_BLOB(aName);
        else if constexpr(valueType == GpReflectType::OBJECT_SP)    return Vec_ObjectSP(aName, T::SReflectModel());
        else
        {
            // if constexpr(valueType == GpReflectType::OBJECT) ...
            // if constexpr(valueType == GpReflectType::ENUM_FLAGS) ...
            // if constexpr(valueType == GpReflectType::ENUM) ...
            // if constexpr(valueType == GpReflectType::BOOLEAN) ...
            // if constexpr(valueType == GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    } else if constexpr(containerType == GpReflectContainerType::VECTOR_WRAP)
    {
        return VecWrap_Object(aName, T::SReflectModel());
    } else if constexpr(containerType == GpReflectContainerType::MAP)
    {
             if constexpr(valueType == GpReflectType::U_INT_8)      return Map_UI8(aName, keyType);
        else if constexpr(valueType == GpReflectType::S_INT_8)      return Map_SI8(aName, keyType);
        else if constexpr(valueType == GpReflectType::U_INT_16)     return Map_UI16(aName, keyType);
        else if constexpr(valueType == GpReflectType::S_INT_16)     return Map_SI16(aName, keyType);
        else if constexpr(valueType == GpReflectType::U_INT_32)     return Map_UI32(aName, keyType);
        else if constexpr(valueType == GpReflectType::S_INT_32)     return Map_SI32(aName, keyType);
        else if constexpr(valueType == GpReflectType::U_INT_64)     return Map_UI64(aName, keyType);
        else if constexpr(valueType == GpReflectType::S_INT_64)     return Map_SI64(aName, keyType);
        else if constexpr(valueType == GpReflectType::DOUBLE)       return Map_Double(aName, keyType);
        else if constexpr(valueType == GpReflectType::FLOAT)        return Map_Float(aName, keyType);
        else if constexpr(valueType == GpReflectType::UUID)         return Map_UUID(aName, keyType);
        else if constexpr(valueType == GpReflectType::STRING)       return Map_String(aName, keyType);
        else if constexpr(valueType == GpReflectType::BLOB)         return Map_BLOB(aName, keyType);
        else if constexpr(valueType == GpReflectType::OBJECT_SP)    return Map_ObjectSP(aName, keyType, T::SReflectModel());
        else
        {
            // if constexpr(GpReflectType::OBJECT) ...
            // if constexpr(GpReflectType::ENUM_FLAGS) ...
            // if constexpr(GpReflectType::ENUM) ...
            // if constexpr(GpReflectType::BOOLEAN) ...
            // if constexpr(GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    } else
    {
        GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
    }

    return *this;
}

template<typename EnumT>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Enum (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnum, EnumT>);

    const size_t align  = alignof(EnumT);
    const size_t size   = sizeof(EnumT);

    iOffset += SCalcOffset(iOffset, align, size);

    static const auto constructFn = [](void* aPropPtr)
    {
        MemOps::SConstruct<EnumT>
        (
            static_cast<EnumT*>(aPropPtr),
            1
        );
    };

    static const auto destructFn = [](void* aPropPtr)
    {
        MemOps::SDestruct<EnumT>
        (
            static_cast<EnumT*>(aPropPtr),
            1
        );
    };

    iProps.emplace_back
    (
        GpReflectProp
        (
            GpReflectType::ENUM,
            GpUUID::CE_Zero(),
            GpReflectContainerType::NO,
            GpReflectType::NOT_SET,
            std::string(aName),
            align,
            size,
            iOffset,
            {},
            {},
            std::nullopt,
            constructFn,
            destructFn,
            {}
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename EnumFlagsT>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::EnumFlags (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnumFlags, EnumFlagsT>);

    const size_t align  = alignof(EnumFlagsT);
    const size_t size   = sizeof(EnumFlagsT);

    iOffset += SCalcOffset(iOffset, align, size);

    static const auto constructFn = [](void* aPropPtr)
    {
        MemOps::SConstruct<EnumFlagsT>
        (
            static_cast<EnumFlagsT*>(aPropPtr),
            1
        );
    };

    static const auto destructFn = [](void* aPropPtr)
    {
        MemOps::SDestruct<EnumFlagsT>
        (
            static_cast<EnumFlagsT*>(aPropPtr),
            1
        );
    };

    iProps.emplace_back
    (
        GpReflectProp
        (
            GpReflectType::ENUM_FLAGS,
            GpUUID::CE_Zero(),
            GpReflectContainerType::NO,
            GpReflectType::NOT_SET,
            std::string(aName),
            align,
            size,
            iOffset,
            {},
            {},
            std::nullopt,
            constructFn,
            destructFn,
            {}
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename T>
ptrdiff_t   GpReflectModelPropBuilder::SCalcOffset (const ptrdiff_t aCurrentOffset) noexcept
{
    return SCalcOffset(aCurrentOffset, alignof(T), sizeof(T));
}

template<typename                       T,
         GpReflectType::EnumT           TE,
         GpReflectContainerType::EnumT  TC>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::_AddProp (std::string_view aName)
{
    size_t align    = 0;
    size_t size     = 0;

    if constexpr (TC == GpReflectContainerType::NO)
    {
        align   = alignof(T);
        size    = sizeof(T);
    } else if constexpr (TC == GpReflectContainerType::VECTOR)
    {
        align   = alignof(std::vector<T>);
        size    = sizeof(std::vector<T>);
    } else if constexpr (TC == GpReflectContainerType::VECTOR_WRAP)
    {
        GpThrowCe<GpException>("Unsupported type for vector wrap container");
    } else
    {
        GpThrowCe<GpException>("Unsupported container");
    }

    iOffset += SCalcOffset(iOffset, align, size);

    const size_t propIdx = std::size(iProps);

    iProps.emplace_back
    (
        GpReflectProp
        (
            TE,
            GpUUID::CE_Zero(),
            TC,
            GpReflectType::NOT_SET,
            std::string(aName),
            align,
            size,
            iOffset,
            {},
            {},
            std::nullopt,
            std::nullopt,
            std::nullopt,
            {},
            propIdx
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename                       T,
         GpReflectType::EnumT           TE,
         GpReflectContainerType::EnumT  TC>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::_AddProp
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    size_t align    = 0;
    size_t size     = 0;

    if constexpr (TC == GpReflectContainerType::NO)
    {
        align   = aModel.Align();
        size    = aModel.Size();
    } else if constexpr (TC == GpReflectContainerType::VECTOR)
    {
        align   = alignof(std::vector<T>);
        size    = sizeof(std::vector<T>);
    } else if constexpr (TC == GpReflectContainerType::VECTOR_WRAP)
    {
        const GpReflectObjectFactory&               factory     = aModel.Factory();
        const GpReflectObjectFactory::VecWrapInfoT& vecWrapInfo = factory.VecWrapInfo();

        align   = vecWrapInfo._align_of;
        size    = vecWrapInfo._size_of;
    } else
    {
        GpThrowCe<GpException>("Unsupported container");
    }

    iOffset += SCalcOffset(iOffset, align, size);

    const size_t propIdx = std::size(iProps);

    iProps.emplace_back
    (
        GpReflectProp
        (
            TE,
            aModel.Uid(),
            TC,
            GpReflectType::NOT_SET,
            std::string(aName),
            align,
            size,
            iOffset,
            {},
            {},
            std::nullopt,
            std::nullopt,
            std::nullopt,
            {},
            propIdx
        )
    );

    iOffset     += size;
    iMaxAlign   = std::max(iMaxAlign, align);

    return *this;
}

template<typename               VT,
         GpReflectType::EnumT   VTE>
GpReflectModelPropBuilder&  GpReflectModelPropBuilder::_AddPropMap
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType,
    const GpUUID&           aModelUid
)
{
    size_t align    = 0;
    size_t size     = 0;

    switch (aKeyType)
    {
        case GpReflectType::U_INT_8:
        {
            align   = alignof(std::map<u_int_8, VT>);
            size    = sizeof(std::map<u_int_8, VT>);
        } break;
        case GpReflectType::S_INT_8:
        {
            align   = alignof(std::map<s_int_8, VT>);
            size    = sizeof(std::map<s_int_8, VT>);
        } break;
        case GpReflectType::U_INT_16:
        {
            align   = alignof(std::map<u_int_16, VT>);
            size    = sizeof(std::map<u_int_16, VT>);
        } break;
        case GpReflectType::S_INT_16:
        {
            align   = alignof(std::map<s_int_16, VT>);
            size    = sizeof(std::map<s_int_16, VT>);
        } break;
        case GpReflectType::U_INT_32:
        {
            align   = alignof(std::map<u_int_32, VT>);
            size    = sizeof(std::map<u_int_32, VT>);
        } break;
        case GpReflectType::S_INT_32:
        {
            align   = alignof(std::map<s_int_32, VT>);
            size    = sizeof(std::map<s_int_32, VT>);
        } break;
        case GpReflectType::U_INT_64:
        {
            align   = alignof(std::map<u_int_64, VT>);
            size    = sizeof(std::map<u_int_64, VT>);
        } break;
        case GpReflectType::S_INT_64:
        {
            align   = alignof(std::map<s_int_64, VT>);
            size    = sizeof(std::map<s_int_64, VT>);
        } break;
        case GpReflectType::DOUBLE:
        {
            align   = alignof(std::map<double, VT>);
            size    = sizeof(std::map<double, VT>);
        } break;
        case GpReflectType::FLOAT:
        {
            align   = alignof(std::map<float, VT>);
            size    = sizeof(std::map<float, VT>);
        } break;
        case GpReflectType::BOOLEAN:
        {
            align   = alignof(std::map<bool, VT>);
            size    = sizeof(std::map<bool, VT>);
        } break;
        case GpReflectType::UUID:
        {
            align   = alignof(std::map<GpUUID, VT>);
            size    = sizeof(std::map<GpUUID, VT>);
        } break;
        case GpReflectType::STRING:
        {
            align   = alignof(std::map<std::string, VT>);
            size    = sizeof(std::map<std::string, VT>);
        } break;
        case GpReflectType::BLOB:       [[fallthrough]];
        case GpReflectType::OBJECT:     [[fallthrough]];
        case GpReflectType::OBJECT_SP:  [[fallthrough]];
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP("Unsupported key type"_sv);
        }
    }

    iOffset += SCalcOffset(iOffset, align, size);

    const size_t propIdx = std::size(iProps);

    iProps.emplace_back
    (
        GpReflectProp
        (
            VTE,
            aModelUid,
            GpReflectContainerType::MAP,
            aKeyType,
            std::string(aName),
            align,
            size,
            iOffset,
            {},
            {},
            std::nullopt,
            std::nullopt,
            std::nullopt,
            {},
            propIdx
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

}// namespace GPlatform
