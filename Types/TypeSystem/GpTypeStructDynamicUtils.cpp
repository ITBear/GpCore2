#include "GpTypeStructDynamicUtils.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include <memory>
#include <cstddef>
#include "../../Utils/RAII/GpRAIIonDestruct.hpp"
#include "GpTypeManager.hpp"
#include "GpTypeStructVisitor.hpp"

namespace GPlatform {

namespace DynamicUtils_SCreate {

//------------------------------------- Visitor_VisitCtx ------------------------------------------

class Visitor_VisitCtx
{
public:
                                            Visitor_VisitCtx    (void) noexcept {}

    [[nodiscard]] bool                      OnVisitBegin        (const GpTypeStructInfo&    aTypeInfo);
    void                                    OnVisitEnd          (const GpTypeStructInfo&    aTypeInfo);
};

bool    Visitor_VisitCtx::OnVisitBegin (const GpTypeStructInfo& /*aTypeInfo*/)
{
    return true;
}

void    Visitor_VisitCtx::OnVisitEnd (const GpTypeStructInfo& /*aTypeInfo*/)
{
}

//------------------------------------- Visitor_VisitValueCtx ------------------------------------------
class Visitor_VisitValueCtx
{
public:
                        Visitor_VisitValueCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt8             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt8             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt16            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt16            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt32            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt32            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt64            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt64            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Double            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Float             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Bool              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UUID              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_String            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_BLOB              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Struct            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_StructSP          (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Enum              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_EnumFlags         (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_UInt8(aStructDataPtr) = u_int_8(0);
}

void    Visitor_VisitValueCtx::Value_SInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_SInt8(aStructDataPtr) = s_int_8(0);
}

void    Visitor_VisitValueCtx::Value_UInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_UInt16(aStructDataPtr) = u_int_16(0);
}

void    Visitor_VisitValueCtx::Value_SInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_SInt16(aStructDataPtr) = s_int_16(0);
}

void    Visitor_VisitValueCtx::Value_UInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_UInt32(aStructDataPtr) = u_int_32(0);
}

void    Visitor_VisitValueCtx::Value_SInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_SInt32(aStructDataPtr) = s_int_32(0);
}

void    Visitor_VisitValueCtx::Value_UInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_UInt64(aStructDataPtr) = u_int_64(0);
}

void    Visitor_VisitValueCtx::Value_SInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_SInt64(aStructDataPtr) = s_int_64(0);
}

void    Visitor_VisitValueCtx::Value_Double
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_Double(aStructDataPtr) = double(0.0);
}

void    Visitor_VisitValueCtx::Value_Float
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_Float(aStructDataPtr) = float(0.0);
}

void    Visitor_VisitValueCtx::Value_Bool
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.Value_Bool(aStructDataPtr) = false;
}

void    Visitor_VisitValueCtx::Value_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<GpUUID>
    (
        &(aPropInfo.Value_UUID(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<std::string>
    (
        &(aPropInfo.Value_String(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<GpBytesArray>
    (
        &(aPropInfo.Value_BLOB(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_Struct
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpTypeStructInfo& typeInfo = GpTypeManager::S().Find(aPropInfo.TypeUID()).value();
    typeInfo.ConstructStruct(&(aPropInfo.Value_Struct(aStructDataPtr)));
}

void    Visitor_VisitValueCtx::Value_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpTypeStructInfo& typeInfo        = GpTypeManager::S().Find(aPropInfo.TypeUID()).value();
    GpTypeStructBase::SP&   structBaseSP    = aPropInfo.Value_StructSP(aStructDataPtr);

    MemOps::SConstruct<GpTypeStructBase::SP>
    (
        &structBaseSP,
        1_cnt
    );

    structBaseSP = typeInfo.NewInstance();
}

void    Visitor_VisitValueCtx::Value_Enum
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.ConstructCustom(aStructDataPtr);
}

void    Visitor_VisitValueCtx::Value_EnumFlags
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.ConstructCustom(aStructDataPtr);
}

//------------------------------------- Visitor_VisitContainerCtx ------------------------------------------
class Visitor_VisitContainerCtx
{
public:
                        Visitor_VisitContainerCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Double                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_String                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Struct                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_StructSP              (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitContainerCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitContainerCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt8(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt8(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt16(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt16(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt32(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt32(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt64(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt64(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Double
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Double(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Float
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Float(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Bool
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Bool(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UUID(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::String(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::BLOB(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Struct
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Structure arrays are not supported; use ::SP instead."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::StructSP(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Enum
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Enum flag arrays are not supported."_sv);
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
                        Visitor_VisitMapCtx     (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_StructSP                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt8(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt8(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt16(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt16(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt32(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt32(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt64(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt64(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Double
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Double(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Float
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Float(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Bool
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Bool(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UUID(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::String(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::StructSP(aStructDataPtr, aPropInfo);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

//------------------------------------- Visitor ------------------------------------------

class Visitor
{
public:
    using VisitCtx          = Visitor_VisitCtx;
    using VisitValueCtx     = Visitor_VisitValueCtx;
    using VisitContainerCtx = Visitor_VisitContainerCtx;
    using VisitMapCtx       = Visitor_VisitMapCtx;
};

//-------------------------------------------------------------------------------

}//namespace DynamicUtils_SCreate

namespace DynamicUtils_SDestroy {

//------------------------------------- Visitor_VisitCtx ------------------------------------------

class Visitor_VisitCtx
{
public:
                                            Visitor_VisitCtx    (void) noexcept {}

    [[nodiscard]] bool                      OnVisitBegin        (const GpTypeStructInfo&    aTypeInfo);
    void                                    OnVisitEnd          (const GpTypeStructInfo&    aTypeInfo);
};

bool    Visitor_VisitCtx::OnVisitBegin (const GpTypeStructInfo& /*aTypeInfo*/)
{
    return true;
}

void    Visitor_VisitCtx::OnVisitEnd (const GpTypeStructInfo& /*aTypeInfo*/)
{
}

//------------------------------------- Visitor_VisitValueCtx ------------------------------------------
class Visitor_VisitValueCtx
{
public:
                        Visitor_VisitValueCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt8             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt8             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt16            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt16            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt32            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt32            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt64            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt64            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Double            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Float             (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Bool              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UUID              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_String            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_BLOB              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Struct            (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_StructSP          (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Enum              (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_EnumFlags         (void*                  aStructDataPtr,
                                                 const GpTypePropInfo&  aPropInfo,
                                                 Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt8
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_SInt8
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt16
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_SInt16
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt32
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_SInt32
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_UInt64
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_SInt64
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Double
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Float
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Bool
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<GpUUID>
    (
        &(aPropInfo.Value_UUID(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<std::string>
    (
        &(aPropInfo.Value_String(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<GpBytesArray>
    (
        &(aPropInfo.Value_BLOB(aStructDataPtr)),
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_Struct
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpTypeStructInfo& typeInfo = GpTypeManager::S().Find(aPropInfo.TypeUID()).value();
    typeInfo.DestructStruct(&(aPropInfo.Value_Struct(aStructDataPtr)));
}

void    Visitor_VisitValueCtx::Value_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    GpTypeStructBase::SP& structBaseSP = aPropInfo.Value_StructSP(aStructDataPtr);

    MemOps::SDestruct<GpTypeStructBase::SP>
    (
        &structBaseSP,
        1_cnt
    );
}

void    Visitor_VisitValueCtx::Value_Enum
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.DestructCustom(aStructDataPtr);
}

void    Visitor_VisitValueCtx::Value_EnumFlags
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aPropInfo.DestructCustom(aStructDataPtr);
}

//------------------------------------- Visitor_VisitContainerCtx ------------------------------------------
class Visitor_VisitContainerCtx
{
public:
                        Visitor_VisitContainerCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Double                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_String                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Struct                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_StructSP              (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitContainerCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitContainerCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt8(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt8(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt16(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt16(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt32(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt32(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt64(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt64(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Double
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Double(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Float
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Float(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Bool
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Bool(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UUID(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::String(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::BLOB(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Struct
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Structure arrays are not supported; use ::SP instead."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::StructSP(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Enum
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GPE("Enum flag arrays are not supported."_sv);
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
                        Visitor_VisitMapCtx     (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_StructSP                  (void*                  aStructDataPtr,
                                                     const GpTypePropInfo&  aPropInfo,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    void*                   /*aStructDataPtr*/,
    const GpTypePropInfo&   /*aPropInfo*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt8(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt8
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt8(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt16(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt16
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt16(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt32(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt32
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt32(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt64(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt64
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt64(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Double
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Double(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Float
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Float(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Bool
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Bool(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UUID
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UUID(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_String
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::String(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_BLOB
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_StructSP
(
    void*                   aStructDataPtr,
    const GpTypePropInfo&   aPropInfo,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::StructSP(aStructDataPtr, aPropInfo);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1_cnt
    );
}

//------------------------------------- Visitor ------------------------------------------

class Visitor
{
public:
    using VisitCtx          = Visitor_VisitCtx;
    using VisitValueCtx     = Visitor_VisitValueCtx;
    using VisitContainerCtx = Visitor_VisitContainerCtx;
    using VisitMapCtx       = Visitor_VisitMapCtx;
};

}//namespace DynamicUtils_SDestroy

GpTypeStructDynamic::SP GpTypeStructDynamicUtils::SConstruct (const GpTypeStructInfo& aTypeInfo)
{
    //Data ptr
    void* structDataPtr = nullptr;

    GpRAIIonDestruct stopGuard([&]()
    {
        if (structDataPtr != nullptr)
        {
            std::free(structDataPtr);
        }

        structDataPtr = nullptr;
    });

    //Props
    const GpTypePropInfo::C::Vec::Val& props = aTypeInfo.Props();

    if (props.size() > 0)
    {
        //Calc size and max align
        const size_t structPropsSize    = aTypeInfo.SizeOfProps();
        const size_t firstPropAlign     = props.at(0).Align();

        //Allocate memory
        //must be a multiple of alignment
        const size_t allocateSize = ((structPropsSize / firstPropAlign) + (structPropsSize % firstPropAlign ? 0 : 1)) * firstPropAlign;
        structDataPtr = std::aligned_alloc(firstPropAlign, allocateSize);

        THROW_GPE_COND
        (
            structDataPtr != nullptr,
            "Memory allocation failed"_sv
        );

        GpTypeStructVisitor<GpTypeStructBase, DynamicUtils_SCreate::Visitor>    visitor;
        DynamicUtils_SCreate::Visitor_VisitCtx ctx;

        visitor.Visit(aTypeInfo, structDataPtr, ctx);
    }

    GpTypeStructDynamic::SP dynamicStructSP = MakeSP<GpTypeStructDynamic>(aTypeInfo, structDataPtr);
    structDataPtr = nullptr;

    return dynamicStructSP;
}

void    GpTypeStructDynamicUtils::SDestroy
(
    const GpTypeStructInfo& aTypeInfo,
    void*                   aStructDataPtr
)
{
    if (aStructDataPtr == nullptr)
    {
        return;
    }

    GpTypeStructVisitor<GpTypeStructBase, DynamicUtils_SDestroy::Visitor>   visitor;
    DynamicUtils_SDestroy::Visitor_VisitCtx ctx;

    visitor.Visit(aTypeInfo, aStructDataPtr, ctx);

    std::free(aStructDataPtr);
}

/*if (aProps.size() == 0)
{
    return {0, 0};
}

void*   ptr         = reinterpret_cast<void*>(alignof(std::max_align_t));
size_t  space       = std::numeric_limits<size_t>::max();
size_t  maxAlign    = 0;


    THROW_GPE_COND
    (
        ptr != nullptr,
        "Failed to calculate align"_sv
    );

    space       -= size;
    ptr         = static_cast<char*>(ptr) + size;
    maxAlign    = std::max(maxAlign, align);
}

return {std::numeric_limits<size_t>::max() - space, maxAlign};*/


}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
