#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectModel.hpp"

#include <memory>

namespace GPlatform {

class GpReflectModelBuilder;

class GP_REFLECTION_API GpReflectPropBuilder
{
public:
    inline                              GpReflectPropBuilder    (void) noexcept;
    inline                              GpReflectPropBuilder    (GpReflectPropBuilder&& aBuilder) noexcept;
    inline                              GpReflectPropBuilder    (GpReflectModelBuilder& aBuilder) noexcept;
                                        ~GpReflectPropBuilder   (void) noexcept;

    inline GpReflectPropBuilder&        operator=               (GpReflectPropBuilder&& aBuilder) noexcept;

    inline static GpReflectPropBuilder  SNew                    (GpReflectModelBuilder& aBuilder) noexcept;

    size_t                              MaxAlign                (void) const noexcept {return iMaxAlign;}
    size_t                              TotalSize               (void) const noexcept {return iTotalSize;}

    GpReflectModelBuilder&              DoneBuildProps          (void);

    inline GpReflectPropBuilder&        SetProps                (GpReflectProp::C::Vec::Val&& aProps);

    GpReflectPropBuilder&               UI8                     (std::string_view aName);
    GpReflectPropBuilder&               SI8                     (std::string_view aName);
    GpReflectPropBuilder&               UI16                    (std::string_view aName);
    GpReflectPropBuilder&               SI16                    (std::string_view aName);
    GpReflectPropBuilder&               UI32                    (std::string_view aName);
    GpReflectPropBuilder&               SI32                    (std::string_view aName);
    GpReflectPropBuilder&               UI64                    (std::string_view aName);
    GpReflectPropBuilder&               SI64                    (std::string_view aName);
    GpReflectPropBuilder&               Double                  (std::string_view aName);
    GpReflectPropBuilder&               Float                   (std::string_view aName);
    GpReflectPropBuilder&               Bool                    (std::string_view aName);
    GpReflectPropBuilder&               Uuid                    (std::string_view aName);
    GpReflectPropBuilder&               String                  (std::string_view aName);
    GpReflectPropBuilder&               Blob                    (std::string_view aName);
    GpReflectPropBuilder&               Object                  (std::string_view       aName,
                                                                 const GpReflectModel&  aModel);
    GpReflectPropBuilder&               ObjectSP                (std::string_view       aName,
                                                                 const GpReflectModel&  aModel);
    template<typename EnumT>
    GpReflectPropBuilder&               Enum                    (std::string_view aName);

    template<typename EnumFlagsT>
    GpReflectPropBuilder&               EnumFlags               (std::string_view aName);

    //Vector
    GpReflectPropBuilder&               Vec_UI8                 (std::string_view aName);
    GpReflectPropBuilder&               Vec_SI8                 (std::string_view aName);
    GpReflectPropBuilder&               Vec_UI16                (std::string_view aName);
    GpReflectPropBuilder&               Vec_SI16                (std::string_view aName);
    GpReflectPropBuilder&               Vec_UI32                (std::string_view aName);
    GpReflectPropBuilder&               Vec_SI32                (std::string_view aName);
    GpReflectPropBuilder&               Vec_UI64                (std::string_view aName);
    GpReflectPropBuilder&               Vec_SI64                (std::string_view aName);
    GpReflectPropBuilder&               Vec_Double              (std::string_view aName);
    GpReflectPropBuilder&               Vec_Float               (std::string_view aName);
    GpReflectPropBuilder&               Vec_Bool                (std::string_view aName);
    GpReflectPropBuilder&               Vec_Uuid                (std::string_view aName);
    GpReflectPropBuilder&               Vec_String              (std::string_view aName);
    GpReflectPropBuilder&               Vec_Blob                (std::string_view aName);
    GpReflectPropBuilder&               Vec_ObjectSP            (std::string_view       aName,
                                                                 const GpReflectModel&  aModel);

    //Map
    GpReflectPropBuilder&               Map_UI8                 (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_SI8                 (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_UI16                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_SI16                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_UI32                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_SI32                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_UI64                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_SI64                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_Double              (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_Float               (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_Bool                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_Uuid                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_String              (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_Blob                (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType);
    GpReflectPropBuilder&               Map_ObjectSP            (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType,
                                                                 const GpReflectModel&  aModel);

    void                                Check                   (void) const;
    GpReflectProp::C::Vec::Val          MoveProps               (void) noexcept {return std::move(iProps);}

private:
    template<typename T>
    [[nodiscard]] static ptrdiff_t      SCalcOffset             (const ptrdiff_t    aCurrentOffset) noexcept;
    [[nodiscard]] static ptrdiff_t      SCalcOffset             (const ptrdiff_t    aCurrentOffset,
                                                                 const size_t       aAlign,
                                                                 const size_t       aSize) noexcept;

    template<typename                       T,
             GpReflectType::EnumT           TE,
             GpReflectContainerType::EnumT  TC>
    GpReflectPropBuilder&               AddProp                 (std::string_view aName);

    template<typename                       T,
             GpReflectType::EnumT           TE,
             GpReflectContainerType::EnumT  TC>
    GpReflectPropBuilder&               AddProp                 (std::string_view       aName,
                                                                 const GpReflectModel&  aModel);

    template<typename                       VT,
             GpReflectType::EnumT           VTE>
    GpReflectPropBuilder&               AddPropMap              (std::string_view       aName,
                                                                 GpReflectType::EnumT   aKeyType,
                                                                 const GpUUID&          aModelUid);

private:
    GpReflectModelBuilder*              iModelBuilder = nullptr;
    GpReflectProp::C::Vec::Val          iProps;
    std::ptrdiff_t                      iOffset     = 0;
    size_t                              iMaxAlign   = 1;
    size_t                              iTotalSize  = 0;
};

GpReflectPropBuilder::GpReflectPropBuilder (void) noexcept
{
}

GpReflectPropBuilder::GpReflectPropBuilder (GpReflectPropBuilder&& aBuilder) noexcept:
iModelBuilder(aBuilder.iModelBuilder),
iProps(std::move(aBuilder.iProps)),
iOffset(std::move(aBuilder.iOffset))
{
}

GpReflectPropBuilder::GpReflectPropBuilder (GpReflectModelBuilder& aBuilder) noexcept:
iModelBuilder(&aBuilder)
{
}

GpReflectPropBuilder&   GpReflectPropBuilder::operator= (GpReflectPropBuilder&& aBuilder) noexcept
{
    iModelBuilder   = aBuilder.iModelBuilder;
    iProps          = std::move(aBuilder.iProps);
    iOffset         = std::move(aBuilder.iOffset);

    return *this;
}

GpReflectPropBuilder    GpReflectPropBuilder::SNew (GpReflectModelBuilder& aBuilder) noexcept
{
    return GpReflectPropBuilder(aBuilder);
}

GpReflectPropBuilder&   GpReflectPropBuilder::SetProps (GpReflectProp::C::Vec::Val&& aProps)
{
    iProps = std::move(aProps);
    return *this;
}

template<typename EnumT>
GpReflectPropBuilder&   GpReflectPropBuilder::Enum (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnum, EnumT>);

    const size_t align  = alignof(EnumT);
    const size_t size   = sizeof(EnumT);

    iOffset += SCalcOffset(iOffset, align, size);

    auto constructFn = [](void* aPropPtr)
    {
        MemOps::SConstruct<EnumT>
        (
            static_cast<EnumT*>(aPropPtr),
            1_cnt
        );
    };

    auto destructFn = [](void* aPropPtr)
    {
        MemOps::SDestruct<EnumT>
        (
            static_cast<EnumT*>(aPropPtr),
            1_cnt
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
            destructFn
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename EnumFlagsT>
GpReflectPropBuilder&   GpReflectPropBuilder::EnumFlags (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnumFlags, EnumFlagsT>);

    const size_t align  = alignof(EnumFlagsT);
    const size_t size   = sizeof(EnumFlagsT);

    iOffset += SCalcOffset(iOffset, align, size);

    auto constructFn = [](void* aPropPtr)
    {
        MemOps::SConstruct<EnumFlagsT>
        (
            static_cast<EnumFlagsT*>(aPropPtr),
            1_cnt
        );
    };

    auto destructFn = [](void* aPropPtr)
    {
        MemOps::SDestruct<EnumFlagsT>
        (
            static_cast<EnumFlagsT*>(aPropPtr),
            1_cnt
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
            destructFn
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename T>
ptrdiff_t   GpReflectPropBuilder::SCalcOffset (const ptrdiff_t  aCurrentOffset) noexcept
{
    return SCalcOffset(aCurrentOffset, alignof(T), sizeof(T));
}

template<typename                       T,
         GpReflectType::EnumT           TE,
         GpReflectContainerType::EnumT  TC>
GpReflectPropBuilder&   GpReflectPropBuilder::AddProp (std::string_view aName)
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
    } else
    {
        GpThrowCe<std::out_of_range>("Unsupported container");
    }

    iOffset += SCalcOffset(iOffset, align, size);

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
            std::nullopt
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename                       T,
         GpReflectType::EnumT           TE,
         GpReflectContainerType::EnumT  TC>
GpReflectPropBuilder&   GpReflectPropBuilder::AddProp
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
    } else
    {
        GpThrowCe<std::out_of_range>("Unsupported container");
    }

    iOffset += SCalcOffset(iOffset, align, size);

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
            std::nullopt
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

template<typename               VT,
         GpReflectType::EnumT   VTE>
GpReflectPropBuilder&   GpReflectPropBuilder::AddPropMap
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
            std::nullopt
        )
    );

    iOffset += size;
    iMaxAlign = std::max(iMaxAlign, align);

    return *this;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_REFLECTION)
