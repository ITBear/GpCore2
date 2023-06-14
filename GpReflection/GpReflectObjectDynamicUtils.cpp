#include "GpReflectObjectDynamicUtils.hpp"

#if defined(GP_USE_REFLECTION)

#include <memory>
#include <cstddef>
#include "GpReflectManager.hpp"
#include "GpReflectVisitor.hpp"
#include "../GpUtils/Other/GpRAIIonDestruct.hpp"

namespace GPlatform {

namespace DynamicUtils_SCreate {

//------------------------------------- Visitor_VisitCtx ------------------------------------------

class Visitor_VisitCtx
{
public:
                                            Visitor_VisitCtx    (void) noexcept {}

    [[nodiscard]] bool                      OnVisitBegin        (const GpReflectModel& aModel);
    void                                    OnVisitEnd          (const GpReflectModel& aModel);
};

bool    Visitor_VisitCtx::OnVisitBegin (const GpReflectModel& /*aModel*/)
{
    return true;
}

void    Visitor_VisitCtx::OnVisitEnd (const GpReflectModel& /*aModel*/)
{
}

//------------------------------------- Visitor_VisitValueCtx ------------------------------------------
class Visitor_VisitValueCtx
{
public:
                        Visitor_VisitValueCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Double            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Float             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Bool              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UUID              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_String            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_BLOB              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Object            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_ObjectSP          (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Enum              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_EnumFlags         (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_UInt8(aDataPtr) = u_int_8(0);
}

void    Visitor_VisitValueCtx::Value_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_SInt8(aDataPtr) = s_int_8(0);
}

void    Visitor_VisitValueCtx::Value_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_UInt16(aDataPtr) = u_int_16(0);
}

void    Visitor_VisitValueCtx::Value_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_SInt16(aDataPtr) = s_int_16(0);
}

void    Visitor_VisitValueCtx::Value_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_UInt32(aDataPtr) = u_int_32(0);
}

void    Visitor_VisitValueCtx::Value_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_SInt32(aDataPtr) = s_int_32(0);
}

void    Visitor_VisitValueCtx::Value_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_UInt64(aDataPtr) = u_int_64(0);
}

void    Visitor_VisitValueCtx::Value_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_SInt64(aDataPtr) = s_int_64(0);
}

void    Visitor_VisitValueCtx::Value_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_Double(aDataPtr) = double(0.0);
}

void    Visitor_VisitValueCtx::Value_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_Float(aDataPtr) = float(0.0);
}

void    Visitor_VisitValueCtx::Value_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.Value_Bool(aDataPtr) = false;
}

void    Visitor_VisitValueCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<GpUUID>
    (
        &(aProp.Value_UUID(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<std::u8string>
    (
        &(aProp.Value_String(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SConstruct<GpBytesArray>
    (
        &(aProp.Value_BLOB(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_Object
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpReflectModel& model = GpReflectManager::S().Find(aProp.ModelUid());
    model.Construct(&(aProp.Value_Object(aDataPtr)));
}

void    Visitor_VisitValueCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpReflectModel&   model       = GpReflectManager::S().Find(aProp.ModelUid());
    GpReflectObject::SP&    objectSP    = aProp.Value_ObjectSP(aDataPtr);

    MemOps::SConstruct<GpReflectObject::SP>(&objectSP, 1);

    objectSP = model.NewInstance();
}

void    Visitor_VisitValueCtx::Value_Enum
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.ConstructCustom(aDataPtr);
}

void    Visitor_VisitValueCtx::Value_EnumFlags
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.ConstructCustom(aDataPtr);
}

//------------------------------------- Visitor_VisitContainerCtx ------------------------------------------
class Visitor_VisitContainerCtx
{
public:
                        Visitor_VisitContainerCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Double                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_String                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Object                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_ObjectSP              (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitContainerCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitContainerCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt8(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt8(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt16(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt16(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt32(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt32(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt64(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt64(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Double(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Float(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Bool(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UUID(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::String(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::BLOB(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Object
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Object arrays are not supported; use Object::SP instead."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::ObjectSP(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Enum
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Enum flag arrays are not supported."_sv);
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
                        Visitor_VisitMapCtx         (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_ObjectSP                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
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
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt8(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt8(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt16(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt16(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt32(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt32(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt64(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt64(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Double(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Float(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Bool(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UUID(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::String(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::ObjectSP(aDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
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

    [[nodiscard]] bool      OnVisitBegin        (const GpReflectModel& aModel);
    void                    OnVisitEnd          (const GpReflectModel& aModel);
};

bool    Visitor_VisitCtx::OnVisitBegin (const GpReflectModel& /*aModel*/)
{
    return true;
}

void    Visitor_VisitCtx::OnVisitEnd (const GpReflectModel& /*aModel*/)
{
}

//------------------------------------- Visitor_VisitValueCtx ------------------------------------------
class Visitor_VisitValueCtx
{
public:
                        Visitor_VisitValueCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_SInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Double            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Float             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Bool              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_UUID              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_String            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_BLOB              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Object            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_ObjectSP          (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_Enum              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
    void                Value_EnumFlags         (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt8
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_SInt8
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt16
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_SInt16
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_UInt32
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_SInt32
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_UInt64
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_SInt64
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Double
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Float
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Value_Bool
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}
void    Visitor_VisitValueCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<GpUUID>
    (
        &(aProp.Value_UUID(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<std::u8string>
    (
        &(aProp.Value_String(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    MemOps::SDestruct<GpBytesArray>
    (
        &(aProp.Value_BLOB(aDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Value_Object
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    const GpReflectModel& model = GpReflectManager::S().Find(aProp.ModelUid());
    model.Destruct(&(aProp.Value_Object(aDataPtr)));
}

void    Visitor_VisitValueCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    GpReflectObject::SP& structBaseSP = aProp.Value_ObjectSP(aDataPtr);

    MemOps::SDestruct<GpReflectObject::SP>
    (
        &structBaseSP,
        1
    );
}

void    Visitor_VisitValueCtx::Value_Enum
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.DestructCustom(aDataPtr);
}

void    Visitor_VisitValueCtx::Value_EnumFlags
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    aProp.DestructCustom(aDataPtr);
}

//------------------------------------- Visitor_VisitContainerCtx ------------------------------------------
class Visitor_VisitContainerCtx
{
public:
                        Visitor_VisitContainerCtx   (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Double                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_String                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Object                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_ObjectSP              (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitContainerCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitContainerCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt8(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt8(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt16(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt16(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt32(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt32(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UInt64(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::SInt64(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Double(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Float(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::Bool(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::UUID(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::String(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::BLOB(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Object
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Object arrays are not supported; use Object::SP instead."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT::ObjectSP(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_Enum
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    THROW_GP(u8"Enum flag arrays are not supported."_sv);
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
                        Visitor_VisitMapCtx     (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_ObjectSP                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
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
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt8(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt8(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt16(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt16(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt32(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt32(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UInt64(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::SInt64(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Double(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Float(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::Bool(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::UUID(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::String(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       /*aCtx*/
)
{
    auto& container = ValGetterT<KeyT>::ObjectSP(aDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
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

GpReflectObjectDynamic::SP  GpReflectObjectDynamicUtils::SConstruct (const GpReflectModel& aModel)
{
    //Data ptr
    void* dataPtr = nullptr;

    GpRAIIonDestruct stopGuard([&]()
    {
        if (dataPtr != nullptr)
        {
            std::free(dataPtr);
        }

        dataPtr = nullptr;
    });

    //Props
    const GpReflectProp::C::Vec::Val& props = aModel.Props();

    if (props.size() > 0)
    {
        //Calc size and max align
        const size_t objectSize     = aModel.Size();
        const size_t objectAlign    = aModel.Align();

        //Allocate memory
        if (objectAlign <= alignof(std::max_align_t))
        {
            dataPtr = std::malloc(objectSize);
        } else
        {
            //must be a multiple of alignment
            const size_t allocateSize = (objectSize / objectAlign) + (objectSize % objectAlign ? 0 : objectAlign);
            dataPtr = std::aligned_alloc(objectAlign, allocateSize);
        }

        THROW_COND_GP
        (
            dataPtr != nullptr,
            "Memory allocation failed"_sv
        );

        GpReflectVisitor<GpReflectObject, DynamicUtils_SCreate::Visitor> visitor;
        DynamicUtils_SCreate::Visitor_VisitCtx ctx;

        visitor.Visit(aModel, dataPtr, ctx);
    }

    GpReflectObjectDynamic::SP dynamicObjectSP = MakeSP<GpReflectObjectDynamic>(aModel.Uid(), dataPtr);
    dataPtr = nullptr;

    return dynamicObjectSP;
}

GpReflectObjectDynamic::SP  GpReflectObjectDynamicUtils::SConstruct (const GpUUID& aModelUid)
{
    const GpReflectModel& model = GpReflectManager::S().Find(aModelUid);
    return SConstruct(model);
}

void    GpReflectObjectDynamicUtils::SDestroy
(
    const GpReflectModel&   aModel,
    void*                   aDataPtr
)
{
    if (aDataPtr == nullptr)
    {
        return;
    }

    GpReflectVisitor<GpReflectObject, DynamicUtils_SDestroy::Visitor> visitor;
    DynamicUtils_SDestroy::Visitor_VisitCtx ctx;

    visitor.Visit(aModel, aDataPtr, ctx);

    std::free(aDataPtr);
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
