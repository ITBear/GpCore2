#include "GpReflectDiffDelta.hpp"

namespace GPlatform {

GpReflectDiffDelta::GpReflectDiffDelta (void) noexcept
{
}

GpReflectDiffDelta::GpReflectDiffDelta (const GpUUID& aReflectModelUid) noexcept:
iReflectModelUid(aReflectModelUid)
{
}

GpReflectDiffDelta::GpReflectDiffDelta (const GpReflectDiffDelta& aDiffDelta):
iReflectModelUid(aDiffDelta.iReflectModelUid),
iDeltaProps     (aDiffDelta.iDeltaProps)
{
}

GpReflectDiffDelta::GpReflectDiffDelta (GpReflectDiffDelta&& aDiffDelta) noexcept:
iReflectModelUid(std::move(aDiffDelta.iReflectModelUid)),
iDeltaProps     (std::move(aDiffDelta.iDeltaProps))
{
}

GpReflectDiffDelta::~GpReflectDiffDelta (void) noexcept
{
}

bool    GpReflectDiffDelta::AddDiff_UI8
(
    const size_t    aPropIdx,
    const u_int_8   aValueFrom,
    const u_int_8   aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_SI8
(
    const size_t    aPropIdx,
    const s_int_8   aValueFrom,
    const s_int_8   aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_UI16
(
    const size_t    aPropIdx,
    const u_int_16  aValueFrom,
    const u_int_16  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_SI16
(
    const size_t    aPropIdx,
    const s_int_16  aValueFrom,
    const s_int_16  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_UI32
(
    const size_t    aPropIdx,
    const u_int_32  aValueFrom,
    const u_int_32  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_SI32
(
    const size_t    aPropIdx,
    const s_int_32  aValueFrom,
    const s_int_32  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_UI64
(
    const size_t    aPropIdx,
    const u_int_64  aValueFrom,
    const u_int_64  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_SI64
(
    const size_t    aPropIdx,
    const s_int_64  aValueFrom,
    const s_int_64  aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_Double
(
    const size_t aPropIdx,
    const double aValueFrom,
    const double aValueTo
)
{
    if (!NumOps::SIsEqual(aValueFrom, aValueTo))
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_Float
(
    const size_t    aPropIdx,
    const float     aValueFrom,
    const float     aValueTo
)
{
    if (!NumOps::SIsEqual(aValueFrom, aValueTo))
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_Bool
(
    const size_t    aPropIdx,
    const bool      aValueFrom,
    const bool      aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_UUID
(
    const size_t    aPropIdx,
    const GpUUID&   aValueFrom,
    const GpUUID&   aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, aValueTo});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_String
(
    const size_t        aPropIdx,
    const std::string&  aValueFrom,
    const std::string&  aValueTo
)
{
    auto diffPropOpt = GpReflectDiffPropString::SCheckAndMake(aValueFrom, aValueTo);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_BLOB
(
    const size_t        aPropIdx,
    const GpBytesArray& aValueFrom,
    const GpBytesArray& aValueTo
)
{
    auto diffPropOpt = GpReflectDiffPropBLOB::SCheckAndMake(aValueFrom, aValueTo);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_Object
(
    const size_t                aPropIdx,
    const GpReflectObject&      aValueFrom,
    const GpReflectObject&      aValueTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    auto diffPropOpt = GpReflectDiffPropObject::SCheckAndMake(aValueFrom, aValueTo, aIsEqualCache, aTotalMemoryUseCache);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_ObjectSP
(
    const size_t                aPropIdx,
    const GpReflectObject::SP&  aValueFrom,
    const GpReflectObject::SP&  aValueTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    auto diffPropOpt = GpReflectDiffPropObjectSP::SCheckAndMake(aValueFrom, aValueTo, aIsEqualCache, aTotalMemoryUseCache);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_Enum
(
    const size_t    aPropIdx,
    const GpEnum&   aValueFrom,
    const GpEnum&   aValueTo
)
{
    auto diffPropOpt = GpReflectDiffPropEnum::SCheckAndMake(aValueFrom, aValueTo);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_EnumFlags
(
    const size_t        aPropIdx,
    const GpEnumFlags&  aValueFrom,
    const GpEnumFlags&  aValueTo
)
{
    auto diffPropOpt = GpReflectDiffPropEnumFlags::SCheckAndMake(aValueFrom, aValueTo);
    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

// --------------------------------------------- Vector ---------------------------------------------

bool    GpReflectDiffDelta::AddDiff_VecUI8
(
    const size_t                aPropIdx,
    const std::vector<u_int_8>& aValueFrom,
    const std::vector<u_int_8>& aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecUI8::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecSI8
(
    const size_t                aPropIdx,
    const std::vector<s_int_8>& aValueFrom,
    const std::vector<s_int_8>& aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecSI8::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecUI16
(
    const size_t                    aPropIdx,
    const std::vector<u_int_16>&    aValueFrom,
    const std::vector<u_int_16>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecUI16::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecSI16
(
    const size_t                    aPropIdx,
    const std::vector<s_int_16>&    aValueFrom,
    const std::vector<s_int_16>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecSI16::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecUI32
(
    const size_t                    aPropIdx,
    const std::vector<u_int_32>&    aValueFrom,
    const std::vector<u_int_32>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecUI32::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecSI32
(
    const size_t                    aPropIdx,
    const std::vector<s_int_32>&    aValueFrom,
    const std::vector<s_int_32>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecSI32::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecUI64
(
    const size_t                    aPropIdx,
    const std::vector<u_int_64>&    aValueFrom,
    const std::vector<u_int_64>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecUI64::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecSI64
(
    const size_t                    aPropIdx,
    const std::vector<s_int_64>&    aValueFrom,
    const std::vector<s_int_64>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecSI64::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecDouble
(
    const size_t                aPropIdx,
    const std::vector<double>&  aValueFrom,
    const std::vector<double>&  aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecDouble::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecFloat
(
    const size_t                aPropIdx,
    const std::vector<float>&   aValueFrom,
    const std::vector<float>&   aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecFloat::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecUUID
(
    const size_t                aPropIdx,
    const std::vector<GpUUID>&  aValueFrom,
    const std::vector<GpUUID>&  aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecUUID::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecString
(
    const size_t                    aPropIdx,
    const std::vector<std::string>& aValueFrom,
    const std::vector<std::string>& aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecString::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecBLOB
(
    const size_t                        aPropIdx,
    const std::vector<GpBytesArray>&    aValueFrom,
    const std::vector<GpBytesArray>&    aValueTo
)
{
    auto diffPropOpt = GpReflectDiffProp_VecBLOB::SCheckAndMake(aValueFrom, aValueTo);

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecObject
(
    const size_t                        aPropIdx,
    const GpVectorReflectObjWrapBase&   aValueFrom,
    const GpVectorReflectObjWrapBase&   aValueTo,
    IsEqualCacheRefOptT&                aIsEqualCache,
    TotalMemoryUseCacheRefOptT&         aTotalMemoryUseCache
)
{
    auto diffPropOpt = GpReflectDiffProp_VecObject::SCheckAndMake
    (
        aValueFrom,
        aValueTo,
        aIsEqualCache,
        aTotalMemoryUseCache
    );

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

bool    GpReflectDiffDelta::AddDiff_VecObjectSP
(
    const size_t                        aPropIdx,
    const GpReflectObject::C::Vec::SP&  aValueFrom,
    const GpReflectObject::C::Vec::SP&  aValueTo,
    IsEqualCacheRefOptT&                aIsEqualCache,
    TotalMemoryUseCacheRefOptT&         aTotalMemoryUseCache
)
{
    auto diffPropOpt = GpReflectDiffProp_VecObjectSP::SCheckAndMake
    (
        aValueFrom,
        aValueTo,
        aIsEqualCache,
        aTotalMemoryUseCache
    );

    if (diffPropOpt.has_value())
    {
        iDeltaProps.emplace_back(DeltaPropT{aPropIdx, std::move(diffPropOpt.value())});
        return true;
    }

    return false;
}

}// namespace GPlatform
