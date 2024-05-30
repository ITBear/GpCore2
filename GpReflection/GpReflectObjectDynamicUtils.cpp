#include "GpReflectObjectDynamicUtils.hpp"

#include "GpReflectManager.hpp"
#include "GpReflectVisitor.hpp"

#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <cstddef>
#include <memory>

namespace GPlatform {

namespace DynamicUtils_SCreate {

//------------------------------------- Visitor_VisitCtx ------------------------------------------

class Visitor_VisitCtx
{
public:
                            Visitor_VisitCtx    (void* aDataPtr) noexcept: iDataPtr(aDataPtr) {}

    [[nodiscard]] bool      OnVisitBegin        (const GpReflectModel& aModel);
    void                    OnVisitEnd          (const GpReflectModel& aModel);

public:
    void*                   iDataPtr = nullptr;
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

    bool        OnVisitBegin            (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        OnVisitEnd              (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI8                     (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI8                     (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI16                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI16                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI32                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI32                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI64                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI64                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Double                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Float                   (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Bool                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UUID                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        String                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        BLOB                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Object                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        ObjectSP                (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Enum                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        EnumFlags               (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::UI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_UI8(aCtx.iDataPtr) = u_int_8(0);
}

void    Visitor_VisitValueCtx::SI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_SI8(aCtx.iDataPtr) = s_int_8(0);
}

void    Visitor_VisitValueCtx::UI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_UI16(aCtx.iDataPtr) = u_int_16(0);
}

void    Visitor_VisitValueCtx::SI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_SI16(aCtx.iDataPtr) = s_int_16(0);
}

void    Visitor_VisitValueCtx::UI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_UI32(aCtx.iDataPtr) = u_int_32(0);
}

void    Visitor_VisitValueCtx::SI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_SI32(aCtx.iDataPtr) = s_int_32(0);
}

void    Visitor_VisitValueCtx::UI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_UI64(aCtx.iDataPtr) = u_int_64(0);
}

void    Visitor_VisitValueCtx::SI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_SI64(aCtx.iDataPtr) = s_int_64(0);
}

void    Visitor_VisitValueCtx::Double
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_Double(aCtx.iDataPtr) = double(0.0);
}

void    Visitor_VisitValueCtx::Float
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_Float(aCtx.iDataPtr) = float(0.0);
}

void    Visitor_VisitValueCtx::Bool
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.Value_Bool(aCtx.iDataPtr) = false;
}

void    Visitor_VisitValueCtx::UUID
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SConstruct<GpUUID>
    (
        &(aProp.Value_UUID(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::String
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SConstruct<std::string>
    (
        &(aProp.Value_String(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::BLOB
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SConstruct<GpBytesArray>
    (
        &(aProp.Value_BLOB(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Object
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aProp.ModelUid());
    modelCSP.Vn().ConstructInplace(&(aProp.Value_Object(aCtx.iDataPtr)));
}

void    Visitor_VisitValueCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    GpReflectModel::CSP     modelCSP = GpReflectManager::S().Find(aProp.ModelUid());
    GpReflectObject::SP&    objectSP = aProp.Value_ObjectSP(aCtx.iDataPtr);

    MemOps::SConstruct<GpReflectObject::SP>(&objectSP, 1);

    objectSP = modelCSP.Vn().NewInstance();
}

void    Visitor_VisitValueCtx::Enum
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.ConstructCustom(aCtx.iDataPtr);
}

void    Visitor_VisitValueCtx::EnumFlags
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.ConstructCustom(aCtx.iDataPtr);
}

//------------------------------------- Visitor_VisitVecCtx ------------------------------------------
class Visitor_VisitVecCtx
{
public:
                Visitor_VisitVecCtx (void) noexcept {}

    bool        OnVisitBegin        (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    void        OnVisitEnd          (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI8                 (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI8                 (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI16                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI16                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI32                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI32                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI64                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI64                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Double              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Float               (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UUID                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        String              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        BLOB                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Object              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        ObjectSP            (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI8(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI8(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI16(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI16(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI32(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI32(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI64(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI64(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Double
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::Double(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Float
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::Float(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UUID
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UUID(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::String
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::String(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::BLOB
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::BLOB(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Object
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    THROW_GP("Object arrays are not supported; use Object::SP instead.");
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::ObjectSP(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
                Visitor_VisitMapCtx (void) noexcept {}

    bool        OnVisitBegin        (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    void        OnVisitEnd          (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI8               (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI8               (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI16              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI16              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI32              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI32              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UI64              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_SI64              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_Double            (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_Float             (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_UUID              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_String            (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_BLOB              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void        K_ObjectSP          (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI8(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI8(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI16(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI16(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI32(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI32(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI64(aCtx.iDataPtr, aProp);

    MemOps::SConstruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI64(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::Double(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::Float(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UUID(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::String(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtr, aProp);

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
    using VisitVecCtx   = Visitor_VisitVecCtx;
    using VisitMapCtx       = Visitor_VisitMapCtx;
};

//-------------------------------------------------------------------------------

}// namespace DynamicUtils_SCreate

namespace DynamicUtils_SDestroy {

//------------------------------------- Visitor_VisitCtx ------------------------------------------

class Visitor_VisitCtx
{
public:
                            Visitor_VisitCtx    (void* aDataPtr) noexcept: iDataPtr(aDataPtr) {}

    [[nodiscard]] bool      OnVisitBegin        (const GpReflectModel& aModel);
    void                    OnVisitEnd          (const GpReflectModel& aModel);

public:
    void*                   iDataPtr = nullptr;
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

    bool        OnVisitBegin            (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        OnVisitEnd              (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI8                     (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI8                     (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI16                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI16                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI32                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI32                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UI64                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        SI64                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Double                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Float                   (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Bool                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        UUID                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        String                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        BLOB                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Object                  (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        ObjectSP                (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        Enum                    (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
    void        EnumFlags               (const GpReflectProp&   aProp,
                                         Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::UI8
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::SI8
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::UI16
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}
void    Visitor_VisitValueCtx::SI16
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::UI32
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}
void    Visitor_VisitValueCtx::SI32
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}
void    Visitor_VisitValueCtx::UI64
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::SI64
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Double
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Float
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

void    Visitor_VisitValueCtx::Bool
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}
void    Visitor_VisitValueCtx::UUID
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SDestruct<GpUUID>
    (
        &(aProp.Value_UUID(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::String
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SDestruct<std::string>
    (
        &(aProp.Value_String(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::BLOB
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    MemOps::SDestruct<GpBytesArray>
    (
        &(aProp.Value_BLOB(aCtx.iDataPtr)),
        1
    );
}

void    Visitor_VisitValueCtx::Object
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aProp.ModelUid());
    modelCSP.Vn().DestructInplace(&(aProp.Value_Object(aCtx.iDataPtr)));
}

void    Visitor_VisitValueCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    GpReflectObject::SP& structBaseSP = aProp.Value_ObjectSP(aCtx.iDataPtr);

    MemOps::SDestruct<GpReflectObject::SP>
    (
        &structBaseSP,
        1
    );
}

void    Visitor_VisitValueCtx::Enum
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.DestructCustom(aCtx.iDataPtr);
}

void    Visitor_VisitValueCtx::EnumFlags
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    aProp.DestructCustom(aCtx.iDataPtr);
}

//------------------------------------- Visitor_VisitVecCtx ------------------------------------------
class Visitor_VisitVecCtx
{
public:
                Visitor_VisitVecCtx (void) noexcept {}

    bool        OnVisitBegin        (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
    void        OnVisitEnd          (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI8                 (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI8                 (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI16                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI16                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI32                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI32                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UI64                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        SI64                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Double              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Float               (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        UUID                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        String              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        BLOB                (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        Object              (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);

    template<typename ValGetterT>
    void        ObjectSP            (const GpReflectProp&   aProp,
                                     Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI8(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI8(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI16(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI16(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI32(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI32(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UI64(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::SI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::SI64(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Double
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::Double(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Float
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::Float(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::UUID
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::UUID(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::String
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::String(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::BLOB
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::BLOB(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::Object
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    THROW_GP("Object arrays are not supported; use Object::SP instead."_sv);
}

template<typename ValGetterT>
void    Visitor_VisitVecCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT::ObjectSP(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

//------------------------------------- Visitor_VisitMapCtx ------------------------------------------
class Visitor_VisitMapCtx
{
public:
            Visitor_VisitMapCtx (void) noexcept {}

    bool    OnVisitBegin        (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
    void    OnVisitEnd          (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI8               (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI8               (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI16              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI16              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI32              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI32              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI64              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI64              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Double            (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Float             (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UUID              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_String            (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_BLOB              (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_ObjectSP          (const GpReflectProp&   aProp,
                                 Visitor_VisitCtx&      aCtx);
};

bool    Visitor_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    return true;
}

void    Visitor_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] Visitor_VisitCtx&      aCtx
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI8(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI8
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI8(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI16(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI16
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI16(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI32(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI32
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI32(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_UI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UI64(aCtx.iDataPtr, aProp);

    MemOps::SDestruct<std::remove_reference_t<decltype(container)>>
    (
        &container,
        1
    );
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    Visitor_VisitMapCtx::K_SI64
(
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::SI64(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::Double(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::Float(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::UUID(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::String(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::BLOB(aCtx.iDataPtr, aProp);

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
    const GpReflectProp&    aProp,
    Visitor_VisitCtx&       aCtx
)
{
    auto& container = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtr, aProp);

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
    using VisitCtx      = Visitor_VisitCtx;
    using VisitValueCtx = Visitor_VisitValueCtx;
    using VisitVecCtx   = Visitor_VisitVecCtx;
    using VisitMapCtx   = Visitor_VisitMapCtx;
};

}// namespace DynamicUtils_SDestroy

GpReflectObjectDynamic::SP  GpReflectObjectDynamicUtils::SConstruct (const GpReflectModel& aModel)
{
    //Data ptr
    void* dataPtr = nullptr;

    GpRAIIonDestruct stopGuard
    (
        [&]()
        {
            if (dataPtr != nullptr)
            {
                std::free(dataPtr);

//#if defined(GP_OS_WINDOWS)
//              _aligned_free(dataPtr);
//#else
//              std::free(dataPtr);
//#endif
            }

            dataPtr = nullptr;
        }
    );

    // Props
    const GpReflectProp::SmallVecVal& props = aModel.Props();

    // Allocate memory for props and initialize it
    if (!props.empty())
    {
        // Calc size and max align
        const size_t objectSize     = aModel.Size();
        const size_t objectAlign    = aModel.Align();

        // Allocate memory for all props
        if (objectAlign <= alignof(std::max_align_t))
        {
            dataPtr = std::malloc(objectSize);
        } else
        {
            THROW_GP("Wrong align");
//          // must be a multiple of alignment
//          const size_t allocateSize = (objectSize / objectAlign) + (objectSize % objectAlign ? 0 : objectAlign);
//
//#if defined(GP_OS_WINDOWS)
//          dataPtr = _aligned_malloc(allocateSize, objectAlign);
//#else
//          dataPtr = std::aligned_alloc(objectAlign, allocateSize);
//#endif
        }

        THROW_COND_GP
        (
            dataPtr != nullptr,
            "Memory allocation failed"_sv
        );

        // Visit all props and initialize it
        GpReflectVisitor<DynamicUtils_SCreate::Visitor> visitor;
        DynamicUtils_SCreate::Visitor_VisitCtx ctx(dataPtr);

        visitor.Visit(aModel, ctx);
    }

    GpReflectObjectDynamic::SP dynamicObjectSP = MakeSP<GpReflectObjectDynamic>(aModel.Uid(), dataPtr);
    dataPtr = nullptr;

    return dynamicObjectSP;
}

GpReflectObjectDynamic::SP  GpReflectObjectDynamicUtils::SConstruct (const GpUUID& aModelUid)
{
    GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aModelUid);
    return SConstruct(modelCSP.Vn());
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

    GpReflectVisitor<DynamicUtils_SDestroy::Visitor>    visitor;
    DynamicUtils_SDestroy::Visitor_VisitCtx             ctx(aDataPtr);

    visitor.Visit(aModel, ctx);

//#if defined(GP_OS_WINDOWS)
//  _aligned_free(aDataPtr);
//#else
//  std::free(aDataPtr);
//#endif
    std::free(aDataPtr);
}

}// namespace GPlatform
