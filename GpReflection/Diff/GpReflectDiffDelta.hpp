#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropString.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropBLOB.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropObject.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropObjectSP.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropEnum.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffPropEnumFlags.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI8.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI8.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI16.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI16.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI32.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI32.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI64.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI64.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecDouble.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecFloat.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUUID.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecString.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecBLOB.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecObject.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecObjectSP.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_Map.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffDelta
{
public:
    CLASS_DD(GpReflectDiffDelta)

    using DeltaPropValueT = std::variant
    <
        // Value
        u_int_8,
        s_int_8,
        u_int_16,
        s_int_16,
        u_int_32,
        s_int_32,
        u_int_64,
        s_int_64,
        double,
        float,
        bool,
        GpUUID,
        GpReflectDiffPropString,
        GpReflectDiffPropBLOB,
        GpReflectDiffPropObject,
        GpReflectDiffPropObjectSP,
        GpReflectDiffPropEnum,
        GpReflectDiffPropEnumFlags,

        // Vec
        GpReflectDiffProp_VecUI8,
        GpReflectDiffProp_VecSI8,
        GpReflectDiffProp_VecUI16,
        GpReflectDiffProp_VecSI16,
        GpReflectDiffProp_VecUI32,
        GpReflectDiffProp_VecSI32,
        GpReflectDiffProp_VecUI64,
        GpReflectDiffProp_VecSI64,
        GpReflectDiffProp_VecDouble,
        GpReflectDiffProp_VecFloat,
        GpReflectDiffProp_VecUUID,
        GpReflectDiffProp_VecString,
        GpReflectDiffProp_VecBLOB,
        GpReflectDiffProp_VecObject,
        GpReflectDiffProp_VecObjectSP,

        // Map
        GpReflectDiffProp_Map
    >;

    using DeltaPropT                    = std::tuple<size_t/*prop idx*/, DeltaPropValueT>;
    using DeltaPropsT                   = std::vector<DeltaPropT>;
    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
                        GpReflectDiffDelta  (void) noexcept;
                        GpReflectDiffDelta  (const GpUUID& aReflectModelUid) noexcept;
                        GpReflectDiffDelta  (const GpReflectDiffDelta& aDiffDelta);
                        GpReflectDiffDelta  (GpReflectDiffDelta&& aDiffDelta) noexcept;
                        ~GpReflectDiffDelta (void) noexcept;

    void                SetReflectModelUid  (const GpUUID& aReflectModelUid) noexcept {iReflectModelUid = aReflectModelUid;}
    const GpUUID&       ReflectModelUid     (void) const noexcept {return iReflectModelUid;}

    const DeltaPropsT&  DeltaProps          (void) const noexcept {return iDeltaProps;}
    DeltaPropsT&        DeltaProps          (void) noexcept {return iDeltaProps;}

    bool                Empty               (void) const noexcept {return iDeltaProps.empty();}

    // ---------------------------- Value (no container) ----------------------------
    bool                AddDiff_UI8         (size_t     aPropIdx,
                                             u_int_8    aValueFrom,
                                             u_int_8    aValueTo);
    bool                AddDiff_SI8         (size_t     aPropIdx,
                                             s_int_8    aValueFrom,
                                             s_int_8    aValueTo);
    bool                AddDiff_UI16        (size_t     aPropIdx,
                                             u_int_16   aValueFrom,
                                             u_int_16   aValueTo);
    bool                AddDiff_SI16        (size_t     aPropIdx,
                                             s_int_16   aValueFrom,
                                             s_int_16   aValueTo);
    bool                AddDiff_UI32        (size_t     aPropIdx,
                                             u_int_32   aValueFrom,
                                             u_int_32   aValueTo);
    bool                AddDiff_SI32        (size_t     aPropIdx,
                                             s_int_32   aValueFrom,
                                             s_int_32   aValueTo);
    bool                AddDiff_UI64        (size_t     aPropIdx,
                                             u_int_64   aValueFrom,
                                             u_int_64   aValueTo);
    bool                AddDiff_SI64        (size_t     aPropIdx,
                                             s_int_64   aValueFrom,
                                             s_int_64   aValueTo);
    bool                AddDiff_Double      (size_t     aPropIdx,
                                             double     aValueFrom,
                                             double     aValueTo);
    bool                AddDiff_Float       (size_t     aPropIdx,
                                             float      aValueFrom,
                                             float      aValueTo);
    bool                AddDiff_Bool        (size_t     aPropIdx,
                                             bool       aValueFrom,
                                             bool       aValueTo);
    bool                AddDiff_UUID        (size_t         aPropIdx,
                                             const GpUUID&  aValueFrom,
                                             const GpUUID&  aValueTo);
    bool                AddDiff_String      (size_t             aPropIdx,
                                             const std::string& aValueFrom,
                                             const std::string& aValueTo);
    bool                AddDiff_BLOB        (size_t                 aPropIdx,
                                             const GpBytesArray&    aValueFrom,
                                             const GpBytesArray&    aValueTo);
    bool                AddDiff_Object      (size_t                         aPropIdx,
                                             const GpReflectObject&         aValueFrom,
                                             const GpReflectObject&         aValueTo,
                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);
    bool                AddDiff_ObjectSP    (size_t                         aPropIdx,
                                             const GpReflectObject::SP&     aValueFrom,
                                             const GpReflectObject::SP&     aValueTo,
                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);
    bool                AddDiff_Enum        (size_t         aPropIdx,
                                             const GpEnum&  aValueFrom,
                                             const GpEnum&  aValueTo);
    bool                AddDiff_EnumFlags   (size_t             aPropIdx,
                                             const GpEnumFlags& aValueFrom,
                                             const GpEnumFlags& aValueTo);

    // ---------------------------- Vector ----------------------------
    bool                AddDiff_VecUI8      (size_t                             aPropIdx,
                                             const std::vector<u_int_8>&        aValueFrom,
                                             const std::vector<u_int_8>&        aValueTo);
    bool                AddDiff_VecSI8      (size_t                             aPropIdx,
                                             const std::vector<s_int_8>&        aValueFrom,
                                             const std::vector<s_int_8>&        aValueTo);
    bool                AddDiff_VecUI16     (size_t                             aPropIdx,
                                             const std::vector<u_int_16>&       aValueFrom,
                                             const std::vector<u_int_16>&       aValueTo);
    bool                AddDiff_VecSI16     (size_t                             aPropIdx,
                                             const std::vector<s_int_16>&       aValueFrom,
                                             const std::vector<s_int_16>&       aValueTo);
    bool                AddDiff_VecUI32     (size_t                             aPropIdx,
                                             const std::vector<u_int_32>&       aValueFrom,
                                             const std::vector<u_int_32>&       aValueTo);
    bool                AddDiff_VecSI32     (size_t                             aPropIdx,
                                             const std::vector<s_int_32>&       aValueFrom,
                                             const std::vector<s_int_32>&       aValueTo);
    bool                AddDiff_VecUI64     (size_t                             aPropIdx,
                                             const std::vector<u_int_64>&       aValueFrom,
                                             const std::vector<u_int_64>&       aValueTo);
    bool                AddDiff_VecSI64     (size_t                             aPropIdx,
                                             const std::vector<s_int_64>&       aValueFrom,
                                             const std::vector<s_int_64>&       aValueTo);
    bool                AddDiff_VecDouble   (size_t                             aPropIdx,
                                             const std::vector<double>&         aValueFrom,
                                             const std::vector<double>&         aValueTo);
    bool                AddDiff_VecFloat    (size_t                             aPropIdx,
                                             const std::vector<float>&          aValueFrom,
                                             const std::vector<float>&          aValueTo);
    bool                AddDiff_VecUUID     (size_t                             aPropIdx,
                                             const std::vector<GpUUID>&         aValueFrom,
                                             const std::vector<GpUUID>&         aValueTo);
    bool                AddDiff_VecString   (size_t                             aPropIdx,
                                             const std::vector<std::string>&    aValueFrom,
                                             const std::vector<std::string>&    aValueTo);
    bool                AddDiff_VecBLOB     (size_t                             aPropIdx,
                                             const std::vector<GpBytesArray>&   aValueFrom,
                                             const std::vector<GpBytesArray>&   aValueTo);
    bool                AddDiff_VecObject   (size_t                             aPropIdx,
                                             const GpVectorReflectObjWrapBase&  aValueFrom,
                                             const GpVectorReflectObjWrapBase&  aValueTo,
                                             IsEqualCacheRefOptT&               aIsEqualCache,
                                             TotalMemoryUseCacheRefOptT&        aTotalMemoryUseCache);
    bool                AddDiff_VecObjectSP (size_t                             aPropIdx,
                                             const GpReflectObject::C::Vec::SP& aValueFrom,
                                             const GpReflectObject::C::Vec::SP& aValueTo,
                                             IsEqualCacheRefOptT&               aIsEqualCache,
                                             TotalMemoryUseCacheRefOptT&        aTotalMemoryUseCache);

    // ---------------------------- Map ----------------------------
    template<typename KeyT,
             typename ValT>
    bool                AddDiff_Map         (size_t                                     aPropIdx,
                                             const std::map<KeyT, ValT, std::less<>>&   aMapFrom,
                                             const std::map<KeyT, ValT, std::less<>>&   aMapTo,
                                             IsEqualCacheRefOptT&                       aIsEqualCache,
                                             TotalMemoryUseCacheRefOptT&                aTotalMemoryUseCache);

private:
    GpUUID              iReflectModelUid;
    DeltaPropsT         iDeltaProps;
};


template<typename KeyT,
         typename ValT>
bool    GpReflectDiffDelta::AddDiff_Map
(
    size_t                                      aPropIdx,
    const std::map<KeyT, ValT, std::less<>>&    aMapFrom,
    const std::map<KeyT, ValT, std::less<>>&    aMapTo,
    IsEqualCacheRefOptT&                        aIsEqualCache,
    TotalMemoryUseCacheRefOptT&                 aTotalMemoryUseCache
)
{
    auto diffPropOpt = GpReflectDiffProp_Map::SCheckAndMake<KeyT, ValT>
    (
        aMapFrom,
        aMapTo,
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
