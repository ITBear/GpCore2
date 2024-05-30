#include "GpReflectUtils_IsEqual.hpp"

#include "GpReflectVisitor.hpp"

namespace GPlatform {

namespace IsEqual {

// ------------------------------------- GpReflectUtils_VisitValueCtx ------------------------------------------
class GpReflectUtils_VisitValueCtx
{
public:
                GpReflectUtils_VisitValueCtx    (void) noexcept {}

    bool        OnVisitBegin    (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        OnVisitEnd      (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        SI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Double          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Float           (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Bool            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        UUID            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        String          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        BLOB            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Object          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        ObjectSP        (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        Enum            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        EnumFlags       (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectUtils_VisitValueCtx::UI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_UI8(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_UI8(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::SI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_SI8(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_SI8(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::UI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_UI16(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_UI16(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::SI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_SI16(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_SI16(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::UI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_UI32(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_UI32(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::SI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_SI32(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_SI32(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::UI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_UI64(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_UI64(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::SI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_SI64(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_SI64(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Double
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_Double(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_Double(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Float
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_Float(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_Float(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Bool
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_Bool(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_Bool(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::UUID
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_UUID(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_UUID(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_String(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_String(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_BLOB(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_BLOB(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Object
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const GpReflectObject& objectA = aProp.Value_Object(aCtx.iReflectDataPtrObjA);
    const GpReflectObject& objectB = aProp.Value_Object(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(objectA, objectB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const GpReflectObject::SP& objectA = aProp.Value_ObjectSP(aCtx.iReflectDataPtrObjA);
    const GpReflectObject::SP& objectB = aProp.Value_ObjectSP(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(objectA, objectB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::Enum
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_Enum(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_Enum(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

void    GpReflectUtils_VisitValueCtx::EnumFlags
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& valA = aProp.Value_EnumFlags(aCtx.iReflectDataPtrObjA);
    const auto& valB = aProp.Value_EnumFlags(aCtx.iReflectDataPtrObjB);

    aCtx.SetIsEqual(GpReflectUtils_IsEqual::SDo(valA, valB, aCtx.iCache));
}

// ------------------------------------- GpReflectUtils_VisitVecCtx ------------------------------------------
class GpReflectUtils_VisitVecCtx
{
public:
                GpReflectUtils_VisitVecCtx  (void) noexcept {}

    bool        OnVisitBegin    (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
    void        OnVisitEnd      (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI8             (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI16            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI32            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        SI64            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Double          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Float           (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        UUID            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        String          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        BLOB            (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        Object          (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void        ObjectSP        (const GpReflectProp&       aProp,
                                 GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::UI8(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::UI8(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::SI8(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::SI8(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::UI16(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::UI16(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::SI16(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::SI16(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::UI32(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::UI32(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::SI32(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::SI32(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::UI64(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::UI64(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::SI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::SI64(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::SI64(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Double
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::Double(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::Double(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpan<const double>(containerA).IsEqual(containerB, NumOps::SIsEqual<const double>));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Float
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::Float(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::Float(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpan<const float>(containerA).IsEqual(containerB, NumOps::SIsEqual<const float>));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::UUID
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::UUID(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::UUID(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual(GpSpanByteR(containerA).IsEqual(containerB));
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::String(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::String(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual
    (
        GpSpan<const std::string>(containerA).IsEqual
        (
            containerB,
            [&aCtx](const std::string& aElementA, const std::string& aElementB)
            {
                return GpReflectUtils_IsEqual::SDo(aElementA, aElementB, aCtx.iCache);
            }
        )
    );
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::BLOB(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::BLOB(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual
    (
        GpSpan<const GpBytesArray>(containerA).IsEqual
        (
            containerB,
            [&aCtx](const GpBytesArray& aElementA, const GpBytesArray& aElementB)
            {
                return GpReflectUtils_IsEqual::SDo(aElementA, aElementB, aCtx.iCache);
            }
        )
    );
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::Object
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::Object(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::Object(aCtx.iReflectDataPtrObjB, aProp);

    const size_t sizeA = std::size(containerA);
    const size_t sizeB = std::size(containerB);

    if (sizeA != sizeB)
    {
        aCtx.SetIsEqual(false);
        return;
    }

    for (size_t id = 0; id < sizeA; id++)
    {
        const GpReflectObject& elementA = containerA[id];
        const GpReflectObject& elementB = containerB[id];

        if (GpReflectUtils_IsEqual::SDo(elementA, elementB, aCtx.iCache) == false)
        {
            aCtx.SetIsEqual(false);
            return;
        }
    }

    aCtx.SetIsEqual(true);
}

template<typename ValGetterT>
void    GpReflectUtils_VisitVecCtx::ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT::ObjectSP(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT::ObjectSP(aCtx.iReflectDataPtrObjB, aProp);

    aCtx.SetIsEqual
    (
        GpSpan<const GpReflectObject::SP>(containerA).IsEqual
        (
            containerB,
            [&aCtx](const GpReflectObject::SP& aElementA, const GpReflectObject::SP& aElementB)
            {
                return GpReflectUtils_IsEqual::SDo(aElementA, aElementB, aCtx.iCache);
            }
        )
    );
}

// ---------------------------------------------- GpReflectUtils_VisitMapCtx ------------------------------------------------------

class GpReflectUtils_VisitMapCtx
{
public:
            GpReflectUtils_VisitMapCtx  (void) noexcept {}

    bool    OnVisitBegin    (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    void    OnVisitEnd      (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI8           (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI8           (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI16          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI16          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI32          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI32          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI64          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI64          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Double        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Float         (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UUID          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_String        (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_BLOB          (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_ObjectSP      (const GpReflectProp&       aProp,
                             GpReflectUtils_VisitCtx&   aCtx);

    template<typename CT>
    void    Process         (const CT&                  aContainerA,
                             const CT&                  aContainerB,
                             GpReflectUtils_VisitCtx&   aCtx);
};

bool    GpReflectUtils_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectUtils_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&       aProp,
    [[maybe_unused]] GpReflectUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::UI8(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::UI8(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI8
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::SI8(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::SI8(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::UI16(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::UI16(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI16
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::SI16(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::SI16(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::UI32(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::UI32(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI32
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::SI32(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::SI32(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::UI64(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::UI64(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_SI64
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::SI64(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::SI64(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_Double
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::Double(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::Double(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_Float
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::Float(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::Float(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_UUID
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::UUID(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::UUID(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_String
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::String(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::String(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_BLOB
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::BLOB(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::BLOB(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectUtils_VisitMapCtx::K_ObjectSP
(
    const GpReflectProp&        aProp,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const auto& containerA = ValGetterT<KeyT>::ObjectSP(aCtx.iReflectDataPtrObjA, aProp);
    const auto& containerB = ValGetterT<KeyT>::ObjectSP(aCtx.iReflectDataPtrObjB, aProp);

    Process(containerA, containerB, aCtx);
}

template<typename CT>
void    GpReflectUtils_VisitMapCtx::Process
(
    const CT&                   aContainerA,
    const CT&                   aContainerB,
    GpReflectUtils_VisitCtx&    aCtx
)
{
    const size_t containerSizeA = std::size(aContainerA);
    const size_t containerSizeB = std::size(aContainerB);

    if (containerSizeA != containerSizeB)
    {
        aCtx.SetIsEqual(false);
        return;
    }

    auto itA = std::begin(aContainerA);
    auto itB = std::begin(aContainerB);

    while (itA != std::end(aContainerA))
    {
        // Compare keys
        if (GpReflectUtils_IsEqual::SDo(itA->first, itB->first, aCtx.iCache) == false)
        {
            aCtx.SetIsEqual(false);
            return;
        }

        // Compare values
        if (GpReflectUtils_IsEqual::SDo(itA->second, itB->second, aCtx.iCache) == false)
        {
            aCtx.SetIsEqual(false);
            return;
        }

        itA++;
        itB++;
    }
}

// ------------------------------------------ GpReflectUtils_Visitor -------------------------------------------------

class GpReflectUtils_Visitor
{
public:
    using VisitCtx      = GpReflectUtils_VisitCtx;
    using VisitValueCtx = GpReflectUtils_VisitValueCtx;
    using VisitVecCtx   = GpReflectUtils_VisitVecCtx;
    using VisitMapCtx   = GpReflectUtils_VisitMapCtx;
};

}// namespace IsEqual

// ------------------------------------------ GpReflectUtils_IsEqual -------------------------------------------------

bool    GpReflectUtils_IsEqual::SDo
(
    const GpReflectObject&  aObjA,
    const GpReflectObject&  aObjB,
    CacheRefOptT            aCache
)
{
    // UIDs must be equal
    if (aObjA.ReflectModelUid() != aObjB.ReflectModelUid()) [[unlikely]]
    {
        return false;
    }

    // Try to find in cache
    if (aCache.has_value())
    {
        auto& cache             = aCache.value().get();
        auto& cacheMultiMap     = std::get<0>(cache);
        auto& cacheIsEqualMap   = std::get<1>(cache);
        const auto multiKey     = cacheMultiMap.AddKeys(&aObjA, &aObjB);
        const auto iter         = cacheIsEqualMap.find(multiKey);

        if (iter != std::end(cacheIsEqualMap))// found in cache
        {
            // return result from cache
            return iter->second;
        }
    }

    GpReflectVisitor<IsEqual::GpReflectUtils_Visitor> visitor;

    IsEqual::GpReflectUtils_VisitCtx ctx
    (
        aObjA.ReflectDataPtr(),
        aObjB.ReflectDataPtr(),
        aCache
    );

    GpReflectModel::CSP modelCSP = aObjA.ReflectModel();

    bool isEqual = true;
    try
    {
        visitor.Visit(modelCSP.Vn(), ctx);
    } catch (IsEqual::GpReflectUtils_NotEqualEx)
    {
        isEqual = false;
    }

    // Update cache
    if (aCache.has_value())
    {
        auto& cache             = aCache.value().get();
        auto& cacheMultiMap     = std::get<0>(cache);
        auto& cacheIsEqualMap   = std::get<1>(cache);
        const auto multiKey     = cacheMultiMap.AddKeys(&aObjA, &aObjB);

        cacheIsEqualMap.emplace(multiKey, isEqual);
    }

    return isEqual;
}

bool    GpReflectUtils_IsEqual::SDo
(
    const GpReflectObject::SP&  aObjA,
    const GpReflectObject::SP&  aObjB,
    CacheRefOptT                aCache
)
{
    if (aObjA.IsNotNULL())
    {
        if (aObjB.IsNotNULL())
        {
            return GpReflectUtils_IsEqual::SDo(aObjA.Vn(), aObjB.Vn(), aCache);
        } else// B is NULL
        {
            return false;
        }
    } else// A is NULL
    {
        if (aObjB.IsNotNULL())
        {
            return false;
        } else// B is NULL
        {
            return true;
        }
    }
}

}// namespace GPlatform
