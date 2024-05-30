#include "GpReflectUtils_AssignValue.hpp"

#include <GpCore2/GpReflection/GpReflectVisitor.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

namespace AssignValue {

void    GpReflectUtils_AssignValue_SDo
(
    GpReflectObject::SP&        aObjDstSP,
    const GpReflectObject::SP&  aObjSrcSP,
    const GpReflectProp&        aProp // base reflect model for both objects
)
{
    if (aObjSrcSP.IsNULL())
    {
        aObjDstSP.Clear();
        return;
    }

    if (aObjDstSP.IsNULL())
    {
        aObjDstSP = GpReflectManager::S().Find(aProp.ModelUid()).Vn().NewInstance();
    }

    GpReflectUtils_AssignValue::SDo(aObjDstSP.Vn(), aObjSrcSP.Vn());
}

// ------------------------------------------ GpReflectPropUtils_VisitCtx -------------------------------------------------
class GpReflectPropUtils_VisitCtx
{
public:
                        GpReflectPropUtils_VisitCtx (void*          aDataPtrDst,
                                                     const void*    aDataPtrSrc) noexcept;

    [[nodiscard]] bool  OnVisitBegin                (const GpReflectModel& aModel);
    void                OnVisitEnd                  (const GpReflectModel& aModel);

public:
    void*               iDataPtrDst = nullptr;
    const void*         iDataPtrSrc = nullptr;
};

GpReflectPropUtils_VisitCtx::GpReflectPropUtils_VisitCtx
(
    void*       aDataPtrDst,
    const void* aDataPtrSrc
) noexcept:
iDataPtrDst(aDataPtrDst),
iDataPtrSrc(aDataPtrSrc)
{
}

bool    GpReflectPropUtils_VisitCtx::OnVisitBegin ([[maybe_unused]] const GpReflectModel& aModel)
{
    return true;
}

void    GpReflectPropUtils_VisitCtx::OnVisitEnd ([[maybe_unused]] const GpReflectModel& aModel)
{
}

// ------------------------------------- GpReflectPropUtils_VisitValueCtx ------------------------------------------
class GpReflectPropUtils_VisitValueCtx
{
public:
            GpReflectPropUtils_VisitValueCtx    (void) noexcept {}

    bool    OnVisitBegin    (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    OnVisitEnd      (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    UI8             (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    SI8             (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    UI16            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    SI16            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    UI32            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    SI32            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    UI64            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    SI64            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    Double          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    Float           (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    Bool            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    UUID            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    String          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    BLOB            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    Object          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    ObjectSP        (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    Enum            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    EnumFlags       (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
};

bool    GpReflectPropUtils_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectPropUtils_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectPropUtils_VisitValueCtx::UI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_UI8(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_UI8(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::SI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_SI8(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_SI8(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::UI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_UI16(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_UI16(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::SI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_SI16(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_SI16(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::UI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_UI32(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_UI32(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::SI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_SI32(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_SI32(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::UI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_UI64(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_UI64(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::SI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_SI64(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_SI64(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::Double
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_Double(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_Double(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::Float
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_Float(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_Float(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::Bool
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_Bool(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_Bool(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::UUID
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_UUID(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_UUID(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::String
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_String(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_String(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::BLOB
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_BLOB(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_BLOB(aCtx.iDataPtrSrc);

    valDst = valSrc;
}

void    GpReflectPropUtils_VisitValueCtx::Object
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_Object(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_Object(aCtx.iDataPtrSrc);

    GpReflectUtils_AssignValue::SDo(valDst, valSrc);
}

void    GpReflectPropUtils_VisitValueCtx::ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDstSP    = aProp.Value_ObjectSP(aCtx.iDataPtrDst);
    const auto& valSrcSP    = aProp.Value_ObjectSP(aCtx.iDataPtrSrc);

    GpReflectUtils_AssignValue_SDo(valDstSP, valSrcSP, aProp);
}

void    GpReflectPropUtils_VisitValueCtx::Enum
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_Enum(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_Enum(aCtx.iDataPtrSrc);

    valDst.FromID(valSrc.ID());
}

void    GpReflectPropUtils_VisitValueCtx::EnumFlags
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valDst  = aProp.Value_EnumFlags(aCtx.iDataPtrDst);
    const auto& valSrc  = aProp.Value_EnumFlags(aCtx.iDataPtrSrc);

    valDst.SetAllFromRaw(valSrc.Value());
}

// ------------------------------------- GpReflectPropUtils_VisitVecCtx ------------------------------------------
class GpReflectPropUtils_VisitVecCtx
{
public:
            GpReflectPropUtils_VisitVecCtx  (void) noexcept {}

    bool    OnVisitBegin    (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    OnVisitEnd      (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI8             (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI8             (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI16            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI16            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI32            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI32            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI64            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI64            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Double          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Float           (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UUID            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    String          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    BLOB            (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Object          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    ObjectSP        (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
};

bool    GpReflectPropUtils_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectPropUtils_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::UI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::UI8(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::UI8(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::SI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::SI8(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::SI8(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::UI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::UI16(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::UI16(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::SI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::SI16(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::SI16(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::UI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::UI32(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::UI32(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::SI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::SI32(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::SI32(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::UI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::UI64(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::UI64(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::SI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::SI8(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::SI8(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::Double
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::Double(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::Double(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::Float
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::Float(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::Float(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::UUID
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::UUID(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::UUID(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::String
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::String(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::String(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::BLOB
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       valVecDst   = ValGetterT::BLOB(aCtx.iDataPtrDst, aProp);
    const auto& valVecSrc   = ValGetterT::BLOB(aCtx.iDataPtrSrc, aProp);

    valVecDst = valVecSrc;
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::Object
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&           valVecDst   = ValGetterT::Object(aCtx.iDataPtrDst, aProp);
    const auto&     valVecSrc   = ValGetterT::Object(aCtx.iDataPtrSrc, aProp);
    const size_t    sizeSrc     = std::size(valVecSrc);

    if (sizeSrc == 0)
    {
        valVecDst.clear();
        valVecDst.shrink_to_fit();
        return;
    }   

    valVecDst.resize(sizeSrc);

    for (size_t id = 0; id < sizeSrc; id++)
    {
        GpReflectUtils_AssignValue::SDo(valVecDst[id], valVecSrc[id]);
    }
}

template<typename ValGetterT>
void    GpReflectPropUtils_VisitVecCtx::ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&           valVecDst   = ValGetterT::ObjectSP(aCtx.iDataPtrDst, aProp);
    const auto&     valVecSrc   = ValGetterT::ObjectSP(aCtx.iDataPtrSrc, aProp);
    const size_t    sizeSrc     = std::size(valVecSrc);

    if (sizeSrc == 0)
    {
        valVecDst.clear();
        valVecDst.shrink_to_fit();
        return;
    }

    valVecDst.resize(sizeSrc);

    for (size_t id = 0; id < sizeSrc; id++)
    {
        GpReflectUtils_AssignValue_SDo(valVecDst[id], valVecSrc[id], aProp);
    }
}

// ---------------------------------------------- GpReflectPropUtils_VisitMapCtx ------------------------------------------------------

class GpReflectPropUtils_VisitMapCtx
{
public:
            GpReflectPropUtils_VisitMapCtx  (void) noexcept {}

    bool    OnVisitBegin    (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    void    OnVisitEnd      (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI8           (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI8           (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI16          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI16          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI32          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI32          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI64          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI64          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Double        (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Float         (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UUID          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_String        (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_BLOB          (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_ObjectSP      (const GpReflectProp&           aProp,
                             GpReflectPropUtils_VisitCtx&   aCtx);

    template<typename CT>
    void    Process         (const CT&                      aContainerA,
                             const CT&                      aContainerB,
                             GpReflectPropUtils_VisitCtx&   aCtx);
};

bool    GpReflectPropUtils_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectPropUtils_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectPropUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_UI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::UI8(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::UI8(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_SI8
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::SI8(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::SI8(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_UI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::UI16(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::UI16(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_SI16
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::SI16(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::SI16(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_UI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::UI32(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::UI32(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_SI32
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::SI32(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::SI32(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_UI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::UI64(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::UI64(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_SI64
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::SI64(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::SI64(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_Double
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::Double(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::Double(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_Float
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::Float(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::Float(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_UUID
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::UUID(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::UUID(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_String
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::String(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::String(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_BLOB
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&       containerVecDst = ValGetterT<KeyT>::BLOB(aCtx.iDataPtrDst, aProp);
    const auto& containerVecSrc = ValGetterT<KeyT>::BLOB(aCtx.iDataPtrSrc, aProp);

    containerVecDst = containerVecSrc;
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectPropUtils_VisitMapCtx::K_ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectPropUtils_VisitCtx&    aCtx
)
{
    auto&           containerVecDst = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtrDst, aProp);
    const auto&     containerVecSrc = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtrSrc, aProp);
    const size_t    sizeSrc         = std::size(containerVecSrc);

    if (sizeSrc == 0)
    {
        containerVecDst.clear();
        return;
    }

    for (const auto& [srcKey, srcValue]: containerVecSrc)
    {
        GpReflectUtils_AssignValue_SDo(containerVecDst[srcKey], srcValue, aProp);
    }
}

// ------------------------------------------ GpReflectDiffUtils_SIsEqual_Visitor -------------------------------------------------

class GpReflectPropUtils_Visitor
{
public:
    using VisitCtx      = GpReflectPropUtils_VisitCtx;
    using VisitValueCtx = GpReflectPropUtils_VisitValueCtx;
    using VisitVecCtx   = GpReflectPropUtils_VisitVecCtx;
    using VisitMapCtx   = GpReflectPropUtils_VisitMapCtx;
};

}// namespace AssignValue

// ------------------------------------------ GpReflectUtils_AssignValue -------------------------------------------------

void    GpReflectUtils_AssignValue::SDo
(
    void*                   aDataPtrDst,
    const GpReflectProp&    aPropDst,
    const void*             aDataPtrSrc,
    const GpReflectProp&    aPropSrc
)
{
    // Check if props are equal
    THROW_COND_GP
    (
        aPropDst.IsEqual(aPropSrc),
        [&]()
        {
            return fmt::format
            (
                "Reflect props does not equal: dst {}, src {}",
                aPropDst.Name(),
                aPropSrc.Name()
            );
        }
    );

    GpReflectVisitor<AssignValue::GpReflectPropUtils_Visitor>   visitor;
    AssignValue::GpReflectPropUtils_VisitCtx                    ctx(aDataPtrDst, aDataPtrSrc);

    visitor.VisitProp(aPropDst, ctx);
}

void    GpReflectUtils_AssignValue::SDo
(
    GpReflectObject&        aObjDst,
    const GpReflectObject&  aObjSrc
)
{
    GpReflectVisitor<AssignValue::GpReflectPropUtils_Visitor>   visitor;
    AssignValue::GpReflectPropUtils_VisitCtx                    ctx(aObjDst.ReflectDataPtr(), aObjSrc.ReflectDataPtr());

    // Get base reflect model for both objects
    GpReflectModel::CSP             reflectModelDstCSP  = aObjDst.ReflectModel();
    GpReflectModel::CSP             reflectModelSrcCSP  = aObjSrc.ReflectModel();
    GpReflectModel::C::Opt::CRef    reflectModelBaseOpt = GpReflectManager::S().SelectBaseModel(reflectModelDstCSP.Vn(), reflectModelSrcCSP.Vn());

    // Check if there are base reflect model for both objects
    THROW_COND_GP
    (
        reflectModelBaseOpt.has_value(),
        [&]()
        {
            return fmt::format
            (
                "There are no base reflect model for both objects: dst {}, src {}",
                reflectModelDstCSP.Vn().Uid(),
                reflectModelSrcCSP.Vn().Uid()
            );
        }
    );

    //
    visitor.Visit(reflectModelBaseOpt.value().get(), ctx);
}

}// namespace GPlatform
