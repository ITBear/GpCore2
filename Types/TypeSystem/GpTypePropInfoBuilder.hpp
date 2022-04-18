#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypePropInfo.hpp"
#include "GpTypeStructDynamicPropDesc.hpp"

#include <memory>

namespace GPlatform {

class GpTypeStructDynamicBuilder;

class GPCORE_API GpTypePropInfoBuilder
{
public:
                                        GpTypePropInfoBuilder   (void) noexcept;
                                        GpTypePropInfoBuilder   (GpTypePropInfoBuilder&& aBuilder) noexcept;

private:
                                        GpTypePropInfoBuilder   (GpTypeStructDynamicBuilder& aStructDynamicBuilder) noexcept;

public:
                                        ~GpTypePropInfoBuilder  (void) noexcept;

    GpTypePropInfoBuilder&              operator=               (GpTypePropInfoBuilder&& aBuilder) noexcept;

    static GpTypePropInfoBuilder        SNew                    (GpTypeStructDynamicBuilder& aStructDynamicBuilder) noexcept {return GpTypePropInfoBuilder(aStructDynamicBuilder);}

    GpTypeStructDynamicBuilder&         DoneBuildProps          (void);

    GpTypePropInfoBuilder&              AddProps                (const GpTypeStructDynamicPropDesc::C::Vec::SP& aProps);

    GpTypePropInfoBuilder&              UI8                     (std::string_view aName);
    GpTypePropInfoBuilder&              SI8                     (std::string_view aName);
    GpTypePropInfoBuilder&              UI16                    (std::string_view aName);
    GpTypePropInfoBuilder&              SI16                    (std::string_view aName);
    GpTypePropInfoBuilder&              UI32                    (std::string_view aName);
    GpTypePropInfoBuilder&              SI32                    (std::string_view aName);
    GpTypePropInfoBuilder&              UI64                    (std::string_view aName);
    GpTypePropInfoBuilder&              SI64                    (std::string_view aName);
    GpTypePropInfoBuilder&              UnixTsS                 (std::string_view aName);
    GpTypePropInfoBuilder&              UnixTsMS                (std::string_view aName);
    GpTypePropInfoBuilder&              Double                  (std::string_view aName);
    GpTypePropInfoBuilder&              Float                   (std::string_view aName);
    GpTypePropInfoBuilder&              Bool                    (std::string_view aName);
    GpTypePropInfoBuilder&              Uuid                    (std::string_view aName);
    GpTypePropInfoBuilder&              String                  (std::string_view aName);
    GpTypePropInfoBuilder&              Blob                    (std::string_view aName);
    GpTypePropInfoBuilder&              Struct                  (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);
    GpTypePropInfoBuilder&              StructSP                (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);
    template<typename EnumT>
    GpTypePropInfoBuilder&              Enum                    (std::string_view aName);

    template<typename EnumFlagsT>
    GpTypePropInfoBuilder&              EnumFlags               (std::string_view aName);

    //Vector
    GpTypePropInfoBuilder&              Vec_UI8                 (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_SI8                 (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_UI16                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_SI16                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_UI32                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_SI32                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_UI64                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_SI64                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_UnixTsS             (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_UnixTsMS            (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_Double              (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_Float               (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_Bool                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_Uuid                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_String              (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_Blob                (std::string_view aName);
    GpTypePropInfoBuilder&              Vec_StructSP            (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);

    //List
    GpTypePropInfoBuilder&              List_UI8                (std::string_view aName);
    GpTypePropInfoBuilder&              List_SI8                (std::string_view aName);
    GpTypePropInfoBuilder&              List_UI16               (std::string_view aName);
    GpTypePropInfoBuilder&              List_SI16               (std::string_view aName);
    GpTypePropInfoBuilder&              List_UI32               (std::string_view aName);
    GpTypePropInfoBuilder&              List_SI32               (std::string_view aName);
    GpTypePropInfoBuilder&              List_UI64               (std::string_view aName);
    GpTypePropInfoBuilder&              List_SI64               (std::string_view aName);
    GpTypePropInfoBuilder&              List_UnixTsS            (std::string_view aName);
    GpTypePropInfoBuilder&              List_UnixTsMS           (std::string_view aName);
    GpTypePropInfoBuilder&              List_Double             (std::string_view aName);
    GpTypePropInfoBuilder&              List_Float              (std::string_view aName);
    GpTypePropInfoBuilder&              List_Bool               (std::string_view aName);
    GpTypePropInfoBuilder&              List_Uuid               (std::string_view aName);
    GpTypePropInfoBuilder&              List_String             (std::string_view aName);
    GpTypePropInfoBuilder&              List_Blob               (std::string_view aName);
    GpTypePropInfoBuilder&              List_StructSP           (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);

    //Set
    GpTypePropInfoBuilder&              Set_UI8                 (std::string_view aName);
    GpTypePropInfoBuilder&              Set_SI8                 (std::string_view aName);
    GpTypePropInfoBuilder&              Set_UI16                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_SI16                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_UI32                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_SI32                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_UI64                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_SI64                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_UnixTsS             (std::string_view aName);
    GpTypePropInfoBuilder&              Set_UnixTsMS            (std::string_view aName);
    GpTypePropInfoBuilder&              Set_Double              (std::string_view aName);
    GpTypePropInfoBuilder&              Set_Float               (std::string_view aName);
    GpTypePropInfoBuilder&              Set_Bool                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_Uuid                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_String              (std::string_view aName);
    GpTypePropInfoBuilder&              Set_Blob                (std::string_view aName);
    GpTypePropInfoBuilder&              Set_StructSP            (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);

    //Map
    GpTypePropInfoBuilder&              Map_UI8                 (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_SI8                 (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_UI16                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_SI16                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_UI32                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_SI32                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_UI64                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_SI64                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_UnixTsS             (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_UnixTsMS            (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_Double              (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_Float               (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_Bool                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_Uuid                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_String              (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_Blob                (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType);
    GpTypePropInfoBuilder&              Map_StructSP            (std::string_view           aName,
                                                                 GpType::EnumT              aKeyType,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);

    GpTypePropInfo::C::Vec::Val         MoveProps               (void) noexcept {return std::move(iProps);}

private:
    template<typename T>
    void                                UpdateOffsetToAlign     (void) noexcept;
    void                                UpdateOffsetToAlign     (const size_t aAlign,
                                                                const size_t aSize) noexcept;

    template<typename               T,
             GpType::EnumT          TE,
             GpTypeContainer::EnumT TC>
    GpTypePropInfoBuilder&              AddProp                 (std::string_view aName);

    template<typename               T,
             GpType::EnumT          TE,
             GpTypeContainer::EnumT TC>
    GpTypePropInfoBuilder&              AddProp                 (std::string_view           aName,
                                                                 const GpTypeStructInfo&    aTypeStructInfo);

    template<typename               VT,
             GpType::EnumT          VTE>
    GpTypePropInfoBuilder&              AddPropMap              (std::string_view   aName,
                                                                 GpType::EnumT      aKeyType,
                                                                 const GpUUID&      aStructUID);

    void                                CheckForProps           (void);

    void                                AddProps_Val            (const GpTypeStructDynamicPropDesc& aPropDesc);
    void                                AddProps_Vec            (const GpTypeStructDynamicPropDesc& aPropDesc);
    void                                AddProps_List           (const GpTypeStructDynamicPropDesc& aPropDesc);
    void                                AddProps_Set            (const GpTypeStructDynamicPropDesc& aPropDesc);
    void                                AddProps_Map            (const GpTypeStructDynamicPropDesc& aPropDesc);

private:
    GpTypeStructDynamicBuilder*         iStructDynamicBuilder = nullptr;
    GpTypePropInfo::C::Vec::Val         iProps;
    std::ptrdiff_t                      iOffset = 0;
};

template<typename EnumT>
GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Enum (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnum, EnumT>);

    const size_t align  = alignof(EnumT);
    const size_t size   = sizeof(EnumT);

    UpdateOffsetToAlign(align, size);

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
        GpTypePropInfo(
            GpType::ENUM,
            GpUUID::CE_Zero(),
            GpTypeContainer::NO,
            GpType::NOT_SET,
            aName,
            align,
            size,
            iOffset,
            {},
            constructFn,
            destructFn
        )
    );

    iOffset += size;

    return *this;
}

template<typename EnumFlagsT>
GpTypePropInfoBuilder&  GpTypePropInfoBuilder::EnumFlags (std::string_view aName)
{
    static_assert(std::is_base_of_v<GpEnumFlags, EnumFlagsT>);

    const size_t align  = alignof(EnumFlagsT);
    const size_t size   = sizeof(EnumFlagsT);

    UpdateOffsetToAlign(align, size);

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
        GpTypePropInfo(
            GpType::ENUM_FLAGS,
            GpUUID::CE_Zero(),
            GpTypeContainer::NO,
            GpType::NOT_SET,
            aName,
            align,
            size,
            iOffset,
            {},
            constructFn,
            destructFn
        )
    );

    iOffset += size;

    return *this;
}

template<typename T>
void    GpTypePropInfoBuilder::UpdateOffsetToAlign (void) noexcept
{
    UpdateOffsetToAlign(alignof(T), sizeof(T));
}

template<typename               T,
         GpType::EnumT          TE,
         GpTypeContainer::EnumT TC>
GpTypePropInfoBuilder&  GpTypePropInfoBuilder::AddProp (std::string_view aName)
{
    size_t align    = 0;
    size_t size     = 0;

    if constexpr (TC == GpTypeContainer::NO)
    {
        align   = alignof(T);
        size    = sizeof(T);
    } else if constexpr (TC == GpTypeContainer::VECTOR)
    {
        align   = alignof(GpVector<T>);
        size    = sizeof(GpVector<T>);
    } else if constexpr (TC == GpTypeContainer::LIST)
    {
        align   = alignof(GpList<T>);
        size    = sizeof(GpList<T>);
    } else if constexpr (TC == GpTypeContainer::SET)
    {
        align   = alignof(GpSet<T>);
        size    = sizeof(GpSet<T>);
    } else
    {
        GpThrowCe<std::out_of_range>("Unsupported container");
    }

    UpdateOffsetToAlign(align, size);

    iProps.emplace_back
    (
        GpTypePropInfo(
            TE,
            GpUUID::CE_Zero(),
            TC,
            GpType::NOT_SET,
            aName,
            align,
            size,
            iOffset,
            {},
            std::nullopt,
            std::nullopt
        )
    );

    iOffset += size;

    return *this;
}

template<typename               T,
         GpType::EnumT          TE,
         GpTypeContainer::EnumT TC>
GpTypePropInfoBuilder&  GpTypePropInfoBuilder::AddProp
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    size_t align    = 0;
    size_t size     = 0;

    if constexpr (TC == GpTypeContainer::NO)
    {
        align   = aTypeStructInfo.Align();
        size    = aTypeStructInfo.Size();
    } else if constexpr (TC == GpTypeContainer::VECTOR)
    {
        align   = alignof(GpVector<T>);
        size    = sizeof(GpVector<T>);
    } else if constexpr (TC == GpTypeContainer::LIST)
    {
        align   = alignof(GpList<T>);
        size    = sizeof(GpList<T>);
    } else if constexpr (TC == GpTypeContainer::SET)
    {
        align   = alignof(GpSet<T>);
        size    = sizeof(GpSet<T>);
    } else
    {
        GpThrowCe<std::out_of_range>("Unsupported container");
    }

    UpdateOffsetToAlign(align, size);

    iProps.emplace_back
    (
        GpTypePropInfo(
            TE,
            aTypeStructInfo.UID(),
            TC,
            GpType::NOT_SET,
            aName,
            align,
            size,
            iOffset,
            {},
            std::nullopt,
            std::nullopt
        )
    );

    iOffset += size;

    return *this;
}

template<typename               VT,
         GpType::EnumT          VTE>
GpTypePropInfoBuilder&  GpTypePropInfoBuilder::AddPropMap
(
    std::string_view    aName,
    GpType::EnumT       aKeyType,
    const GpUUID&       aStructUID
)
{
    size_t align    = 0;
    size_t size     = 0;

    switch (aKeyType)
    {
        case GpType::U_INT_8:
        {
            align   = alignof(GpMap<u_int_8, VT>);
            size    = sizeof(GpMap<u_int_8, VT>);
        } break;
        case GpType::S_INT_8:
        {
            align   = alignof(GpMap<s_int_8, VT>);
            size    = sizeof(GpMap<s_int_8, VT>);
        } break;
        case GpType::U_INT_16:
        {
            align   = alignof(GpMap<u_int_16, VT>);
            size    = sizeof(GpMap<u_int_16, VT>);
        } break;
        case GpType::S_INT_16:
        {
            align   = alignof(GpMap<s_int_16, VT>);
            size    = sizeof(GpMap<s_int_16, VT>);
        } break;
        case GpType::U_INT_32:
        {
            align   = alignof(GpMap<u_int_32, VT>);
            size    = sizeof(GpMap<u_int_32, VT>);
        } break;
        case GpType::S_INT_32:
        {
            align   = alignof(GpMap<s_int_32, VT>);
            size    = sizeof(GpMap<s_int_32, VT>);
        } break;
        case GpType::U_INT_64:
        {
            align   = alignof(GpMap<u_int_64, VT>);
            size    = sizeof(GpMap<u_int_64, VT>);
        } break;
        case GpType::S_INT_64:
        case GpType::UNIX_TS_S:
        case GpType::UNIX_TS_MS:
        {
            align   = alignof(GpMap<s_int_64, VT>);
            size    = sizeof(GpMap<s_int_64, VT>);
        } break;
        case GpType::DOUBLE:
        {
            align   = alignof(GpMap<double, VT>);
            size    = sizeof(GpMap<double, VT>);
        } break;
        case GpType::FLOAT:
        {
            align   = alignof(GpMap<float, VT>);
            size    = sizeof(GpMap<float, VT>);
        } break;
        case GpType::BOOLEAN:
        {
            align   = alignof(GpMap<bool, VT>);
            size    = sizeof(GpMap<bool, VT>);
        } break;
        case GpType::UUID:
        {
            align   = alignof(GpMap<GpUUID, VT>);
            size    = sizeof(GpMap<GpUUID, VT>);
        } break;
        case GpType::STRING:
        {
            align   = alignof(GpMap<std::string, VT>);
            size    = sizeof(GpMap<std::string, VT>);
        } break;
        case GpType::BLOB: [[fallthrough]];
        case GpType::STRUCT: [[fallthrough]];
        case GpType::STRUCT_SP: [[fallthrough]];
        case GpType::ENUM: [[fallthrough]];
        case GpType::ENUM_FLAGS: [[fallthrough]];
        case GpType::NOT_SET: [[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported key type"_sv);
        }
    }

    UpdateOffsetToAlign(align, size);

    iProps.emplace_back
    (
        GpTypePropInfo(
            VTE,
            aStructUID,
            GpTypeContainer::MAP,
            aKeyType,
            aName,
            align,
            size,
            iOffset,
            {},
            std::nullopt,
            std::nullopt
        )
    );

    iOffset += size;

    return *this;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
