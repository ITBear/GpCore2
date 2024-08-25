#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpMultiKeyManager.hpp>

#include "GpReflectObject.hpp"

namespace GPlatform {

namespace IsEqual {
class GpReflectUtils_VisitCtx;
}// namespace IsEqual

class GP_REFLECTION_API GpReflectUtils_IsEqual
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils_IsEqual)

    using CacheMKeyManagerT = GpMultiKeyManager<u_int_32/*multikey*/,
                                                boost::container::small_flat_map<const GpReflectObject*, u_int_16/*internal index*/, 64>>;
    using CacheMapT         = boost::container::small_flat_map<u_int_32/*multikey*/, bool/*is equal*/, 32>;
    using CacheT            = std::tuple<CacheMKeyManagerT, CacheMapT>;
    using CacheRefOptT      = std::optional<std::reference_wrapper<CacheT>>;

public:
    static bool     SDo (const GpReflectObject& aObjA,
                         const GpReflectObject& aObjB,
                         CacheRefOptT           aCache);

    static bool     SDo (const GpReflectObject::SP& aObjA,
                         const GpReflectObject::SP& aObjB,
                         CacheRefOptT               aCache);

    template<typename T>
    static bool     SDo (const T&                       aValueA,
                         const T&                       aValueB,
                         [[maybe_unused]] CacheRefOptT& aCache);
};

namespace IsEqual {

enum class GpReflectUtils_NotEqualEx
{
    NOT_EQUAL
};

class GpReflectUtils_VisitCtx
{
public:
    using CacheRefOptT = GpReflectUtils_IsEqual::CacheRefOptT;

public:
    inline          GpReflectUtils_VisitCtx (const void*    aReflectDataPtrObjA,
                                             const void*    aReflectDataPtrObjB,
                                             CacheRefOptT&  aCache) noexcept;

    inline bool     OnVisitBegin            (const GpReflectModel& aModel);
    inline void     OnVisitEnd              (const GpReflectModel& aModel);

    inline void     SetIsEqual              (bool aIsEqual);

public:
    const void*     iReflectDataPtrObjA = nullptr;
    const void*     iReflectDataPtrObjB = nullptr;
    CacheRefOptT&   iCache;
};

GpReflectUtils_VisitCtx::GpReflectUtils_VisitCtx
(
    const void*     aReflectDataPtrObjA,
    const void*     aReflectDataPtrObjB,
    CacheRefOptT&   aCache
) noexcept:
iReflectDataPtrObjA(aReflectDataPtrObjA),
iReflectDataPtrObjB(aReflectDataPtrObjB),
iCache             (aCache)
{
}

bool    GpReflectUtils_VisitCtx::OnVisitBegin ([[maybe_unused]] const GpReflectModel& aModel)
{
    return true;
}

void    GpReflectUtils_VisitCtx::OnVisitEnd ([[maybe_unused]] const GpReflectModel& aModel)
{
    // NOP
}

void    GpReflectUtils_VisitCtx::SetIsEqual (bool aIsEqual)
{
    if (!aIsEqual)
    {
        throw GpReflectUtils_NotEqualEx{};
    }
}

}// namespace IsEqual

template<typename T>
bool GpReflectUtils_IsEqual::SDo
(
    const T&                        aValueA,
    const T&                        aValueB,
    [[maybe_unused]] CacheRefOptT&  aCache
)
{
    constexpr const GpReflectType::EnumT type = GpReflectUtils::SDetectType<T>();

    if constexpr (type == GpReflectType::U_INT_8)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::S_INT_8)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::U_INT_16)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::S_INT_16)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::U_INT_32)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::S_INT_32)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::U_INT_64)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::S_INT_64)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::DOUBLE)
    {
        return NumOps::SIsEqual(aValueA, aValueB);
    } else if constexpr (type == GpReflectType::FLOAT)
    {
        return NumOps::SIsEqual(aValueA, aValueB);
    } else if constexpr (type == GpReflectType::BOOLEAN)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::UUID)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::STRING)
    {
        return aValueA == aValueB;
    } else if constexpr (type == GpReflectType::BLOB)
    {
        return MemOps::SIsEqual(std::data(aValueA), std::size(aValueA), std::data(aValueB), std::size(aValueB));
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        return GpReflectUtils_IsEqual::SDo(aValueA, aValueB, aCache);
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        return GpReflectUtils_IsEqual::SDo(aValueA, aValueB, aCache);
    } else if constexpr (type == GpReflectType::ENUM)
    {
        return aValueA.ID() == aValueB.ID();
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        return aValueA.Value() == aValueB.Value();
    } else
    {
        GpThrowCe<GpException>("Unsupported container");
    }
}

}// namespace GPlatform
