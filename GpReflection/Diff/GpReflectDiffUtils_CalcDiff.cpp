#include "GpReflectDiffUtils_CalcDiff.hpp"

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

#include "../GpReflectManager.hpp"
#include "../GpReflectVisitor.hpp"

#include "GpReflectDiffDelta.hpp"

namespace GPlatform {

namespace CalcDiff {

// ------------------------------------------ GpReflectDiffUtils_VisitCtx -------------------------------------------------
class GpReflectDiffUtils_VisitCtx
{
public:
    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
                                GpReflectDiffUtils_VisitCtx (const void*                    aReflectDataPtrObjFrom,
                                                             const void*                    aReflectDataPtrObjTo,
                                                             const GpUUID&                  aReflectModelUidTo,
                                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache) noexcept;

    [[nodiscard]] bool          OnVisitBegin                (const GpReflectModel& aModel);
    void                        OnVisitEnd                  (const GpReflectModel& aModel);
    GpReflectDiffDelta::SP      DiffDelta                   (void) {return iDiffDelta;}

public:
    const void*                 iReflectDataPtrObjFrom  = nullptr;
    const void*                 iReflectDataPtrObjTo    = nullptr;
    GpReflectDiffDelta::SP      iDiffDelta;
    GpUUID                      iReflectModelUidTo;
    IsEqualCacheRefOptT&        iIsEqualCache;
    TotalMemoryUseCacheRefOptT& iTotalMemoryUseCache;
};

GpReflectDiffUtils_VisitCtx::GpReflectDiffUtils_VisitCtx
(
    const void*                 aReflectDataPtrObjFrom,
    const void*                 aReflectDataPtrObTo,
    const GpUUID&               aReflectModelUidTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
) noexcept:
iReflectDataPtrObjFrom(aReflectDataPtrObjFrom),
iReflectDataPtrObjTo  (aReflectDataPtrObTo),
iReflectModelUidTo    (aReflectModelUidTo),
iIsEqualCache         (aIsEqualCache),
iTotalMemoryUseCache  (aTotalMemoryUseCache)
{
}

bool    GpReflectDiffUtils_VisitCtx::OnVisitBegin ([[maybe_unused]] const GpReflectModel& aModel)
{
    if (iDiffDelta.IsNULL())
    {
        iDiffDelta = MakeSP<GpReflectDiffDelta>(iReflectModelUidTo);
    }

    return true;
}

void    GpReflectDiffUtils_VisitCtx::OnVisitEnd ([[maybe_unused]] const GpReflectModel& aModel)
{
}

// ------------------------------------- GpReflectDiffUtils_VisitValueCtx ------------------------------------------
class GpReflectDiffUtils_VisitValueCtx
{
public:
                GpReflectDiffUtils_VisitValueCtx    (void) noexcept {}

    bool        OnVisitBegin    (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        OnVisitEnd      (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        UI8             (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        SI8             (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        UI16            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        SI16            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        UI32            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        SI32            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        UI64            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        SI64            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        Double          (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        Float           (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        Bool            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        UUID            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        String          (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        BLOB            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        Object          (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        ObjectSP        (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        Enum            (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
    void        EnumFlags       (const GpReflectProp&           aProp,
                                 GpReflectDiffUtils_VisitCtx&   aCtx);
};

bool    GpReflectDiffUtils_VisitValueCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectDiffUtils_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    // NOP
}

void    GpReflectDiffUtils_VisitValueCtx::UI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_UI8(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_UI8(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_UI8(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::SI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_SI8(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_SI8(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_SI8(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::UI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_UI16(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_UI16(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_UI16(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::SI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_SI16(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_SI16(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_SI16(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::UI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_UI32(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_UI32(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_UI32(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::SI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_SI32(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_SI32(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_SI32(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::UI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_UI64(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_UI64(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_UI64(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::SI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_SI64(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_SI64(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_SI64(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::Double
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_Double(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_Double(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_Double(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::Float
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_Float(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_Float(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_Float(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::Bool
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_Bool(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_Bool(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_Bool(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::UUID
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_UUID(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_UUID(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_UUID(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::String
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_String(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_String(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_String(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::BLOB
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_BLOB(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_BLOB(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_BLOB(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::Object
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_Object(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_Object(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_Object(aProp.IdxInProps(), valFrom, valTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

void    GpReflectDiffUtils_VisitValueCtx::ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_ObjectSP(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_ObjectSP(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_ObjectSP(aProp.IdxInProps(), valFrom, valTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

void    GpReflectDiffUtils_VisitValueCtx::Enum
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_Enum(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_Enum(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_Enum(aProp.IdxInProps(), valFrom, valTo);
}

void    GpReflectDiffUtils_VisitValueCtx::EnumFlags
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& valFrom = aProp.Value_EnumFlags(aCtx.iReflectDataPtrObjFrom);
    const auto& valTo   = aProp.Value_EnumFlags(aCtx.iReflectDataPtrObjTo);

    aCtx.iDiffDelta.Vn().AddDiff_EnumFlags(aProp.IdxInProps(), valFrom, valTo);
}

// ------------------------------------- GpReflectDiffUtils_VisitVecCtx ------------------------------------------
class GpReflectDiffUtils_VisitVecCtx
{
public:
    GpReflectDiffUtils_VisitVecCtx  (void) noexcept {}

    bool    OnVisitBegin(const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);
    void    OnVisitEnd  (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);
    template<typename ValGetterT>
    void    UI8         (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI8         (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI16        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI16        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI32        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI32        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UI64        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    SI64        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Double      (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Float       (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    UUID        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    String      (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    BLOB        (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    Object      (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename ValGetterT>
    void    ObjectSP    (const GpReflectProp&           aProp,
                         GpReflectDiffUtils_VisitCtx&   aCtx);
};

bool    GpReflectDiffUtils_VisitVecCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectDiffUtils_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::UI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::UI8(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::UI8(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecUI8(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::SI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::SI8(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::SI8(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecSI8(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::UI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::UI16(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::UI16(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecUI16(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::SI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::SI16(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::SI16(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecSI16(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::UI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::UI32(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::UI32(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecUI32(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::SI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::SI32(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::SI32(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecSI32(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::UI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::UI64(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::UI64(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecUI64(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::SI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::SI64(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::SI64(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecSI64(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::Double
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::Double(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::Double(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecDouble(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::Float
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::Float(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::Float(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecFloat(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::UUID
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::UUID(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::UUID(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecUUID(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::String
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::String(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::String(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecString(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::BLOB
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::BLOB(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::BLOB(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecBLOB(aProp.IdxInProps(), vecFrom, vecTo);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::Object
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::Object(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::Object(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecObject(aProp.IdxInProps(), vecFrom, vecTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename ValGetterT>
void    GpReflectDiffUtils_VisitVecCtx::ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& vecFrom = ValGetterT::ObjectSP(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& vecTo   = ValGetterT::ObjectSP(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_VecObjectSP(aProp.IdxInProps(), vecFrom, vecTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

// ------------------------------------------ GpReflectDiffUtils_VisitMapCtx -------------------------------------------------

class GpReflectDiffUtils_VisitMapCtx
{
public:
            GpReflectDiffUtils_VisitMapCtx(void) noexcept {}

    bool    OnVisitBegin    (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
    void    OnVisitEnd      (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI8           (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI8           (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI16          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI16          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI32          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI32          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI64          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI64          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Double        (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Float         (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UUID          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_String        (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_BLOB          (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_ObjectSP      (const GpReflectProp&           aProp,
                             GpReflectDiffUtils_VisitCtx&   aCtx);
};

bool    GpReflectDiffUtils_VisitMapCtx::OnVisitBegin
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    return true;
}

void    GpReflectDiffUtils_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&           aProp,
    [[maybe_unused]] GpReflectDiffUtils_VisitCtx&   aCtx
)
{
    // NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_UI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::UI8(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::UI8(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_SI8
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::SI8(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::SI8(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_UI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::UI16(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::UI16(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_SI16
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::SI16(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::SI16(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_UI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::UI32(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::UI32(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_SI32
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::SI32(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::SI32(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_UI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::UI64(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::UI64(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_SI64
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::SI64(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::SI64(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_Double
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::Double(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::Double(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_Float
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::Float(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::Float(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_UUID
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::UUID(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::UUID(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_String
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::String(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::String(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_BLOB
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::BLOB(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::BLOB(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    GpReflectDiffUtils_VisitMapCtx::K_ObjectSP
(
    const GpReflectProp&            aProp,
    GpReflectDiffUtils_VisitCtx&    aCtx
)
{
    const auto& mapFrom = ValGetterT<KeyT>::ObjectSP(aCtx.iReflectDataPtrObjFrom, aProp);
    const auto& mapTo   = ValGetterT<KeyT>::ObjectSP(aCtx.iReflectDataPtrObjTo, aProp);

    aCtx.iDiffDelta.Vn().AddDiff_Map<KeyT ,ValT>(aProp.IdxInProps(), mapFrom, mapTo, aCtx.iIsEqualCache, aCtx.iTotalMemoryUseCache);
}

// ------------------------------------------ GpReflectDiffUtils_Visitor -------------------------------------------------

class GpReflectDiffUtils_Visitor
{
public:
    using VisitCtx      = GpReflectDiffUtils_VisitCtx;
    using VisitValueCtx = GpReflectDiffUtils_VisitValueCtx;
    using VisitVecCtx   = GpReflectDiffUtils_VisitVecCtx;
    using VisitMapCtx   = GpReflectDiffUtils_VisitMapCtx;
};

}// namespace CalcDiff

// ------------------------------------------ GpReflectDiffUtils_CalcDiff -------------------------------------------------

GpReflectDiffDelta::SP  GpReflectDiffUtils_CalcDiff::SDo
(
    const GpReflectObject&      aObjFrom,
    const GpReflectObject&      aObjTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    // Get base reflect model for both objects
    GpReflectModel::CSP                 modelFromCSP    = aObjFrom.ReflectModel();
    GpReflectModel::CSP                 modelToCSP      = aObjTo.ReflectModel();
    const GpReflectModel&               modelTo         = modelToCSP.Vn();
    const GpReflectModel::C::Opt::CRef  modelBaseOpt    = GpReflectManager::S().SelectBaseModel(modelFromCSP.Vn(), modelTo);
    const GpUUID                        modelToUid      = modelToCSP.Vn().Uid();

    // Check if there are base reflect model for both objects
    THROW_COND_GP
    (
        modelBaseOpt.has_value(),
        [&]()
        {
            return fmt::format
            (
                "There are no base reflect model for both objects: from {}, to {}",
                modelFromCSP.Vn().Uid(),
                modelToUid
            );
        }
    );

    // Collect diff (common part)
    const GpReflectModel& modelBase = modelBaseOpt.value();

    GpReflectVisitor<CalcDiff::GpReflectDiffUtils_Visitor> visitor;

    CalcDiff::GpReflectDiffUtils_VisitCtx ctx
    (
        aObjFrom.ReflectDataPtr(),
        aObjTo.ReflectDataPtr(),
        modelToUid,
        aIsEqualCache,
        aTotalMemoryUseCache
    );

    visitor.Visit(modelBase, ctx);

    // Collect TO props above base model
    if (   (modelBase.Uid() != modelToUid)
        && (GpReflectManager::S().IsBaseOf(modelBase.Uid(), modelToUid)))
    {
        // Get props above base model
        GpReflectProp::SmallVecCRef props = modelTo.PropsWithoutBaseProps();
        if (!props.empty())
        {
            // Create default TO object
            GpReflectObject::SP defaultObjTo = modelTo.NewInstance();
            ctx.iReflectDataPtrObjFrom = defaultObjTo.Vn().ReflectDataPtr();

            // Visit props
            for (const auto& prop: props)
            {
                visitor.VisitProp(prop, ctx);
            }
        }
    }

    return ctx.DiffDelta();
}

}// namespace GPlatform
