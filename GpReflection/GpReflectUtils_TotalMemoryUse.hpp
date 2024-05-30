#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpMultiKeyManager.hpp>

#include "GpReflectObject.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectUtils_TotalMemoryUse
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils_TotalMemoryUse)

    using CacheT        = boost::container::small_flat_map<const GpReflectObject*, size_t, 32>;
    using CacheRefOptT  = std::optional<std::reference_wrapper<CacheT>>;

    enum Mode
    {
        COUNT_SELF_SIZE,
        DONT_COUNT_SELF_SIZE
    };

public:
    static size_t           SDo                 (const GpReflectObject& aObject,
                                                 CacheRefOptT           aCache,
                                                 Mode                   aMode = Mode::COUNT_SELF_SIZE);

    template<typename T>
    static constexpr size_t SSizeofAndAllocated (const T&                                       aValue,
                                                 GpReflectUtils_TotalMemoryUse::CacheRefOptT&   aCache);

    template<typename T>
    static size_t           SGetAllocatedSize   (const T&                                       aValue,
                                                 GpReflectUtils_TotalMemoryUse::CacheRefOptT&   aCache);
};

template<typename T>
constexpr size_t    GpReflectUtils_TotalMemoryUse::SSizeofAndAllocated
(
    [[maybe_unused]] const T&                                       aValue,
    [[maybe_unused]] GpReflectUtils_TotalMemoryUse::CacheRefOptT&   aCache
)
{
    constexpr const GpReflectType::EnumT reflectType = GpReflectUtils::SDetectType<T>();

    if constexpr (reflectType == GpReflectType::STRING)
    {
        return SGetAllocatedSize(aValue, aCache);
    } else if constexpr (reflectType == GpReflectType::BLOB)
    {
        return SGetAllocatedSize(aValue, aCache);
    } else if constexpr (reflectType == GpReflectType::OBJECT)
    {
        return SGetAllocatedSize(aValue, aCache);
    } else if constexpr (reflectType == GpReflectType::OBJECT_SP)
    {
        return SGetAllocatedSize(aValue, aCache);
    } else
    {
        return sizeof(T);
    }
}

template<typename T>
size_t GpReflectUtils_TotalMemoryUse::SGetAllocatedSize
(
    [[maybe_unused]] const T&                                       aValue,
    [[maybe_unused]] GpReflectUtils_TotalMemoryUse::CacheRefOptT&   aCache
)
{
    constexpr const GpReflectType::EnumT type = GpReflectUtils::SDetectType<T>();

    size_t allocatedSize = 0;

    if constexpr (type == GpReflectType::STRING)
    {
        allocatedSize = std::size(aValue);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        if (std::data(aValue))
        {
            allocatedSize = std::size(aValue);
        }
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        allocatedSize = GpReflectUtils_TotalMemoryUse::SDo(aValue, aCache, GpReflectUtils_TotalMemoryUse::Mode::DONT_COUNT_SELF_SIZE);
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        if (aValue.IsNotNULL())
        {
            allocatedSize = GpReflectUtils_TotalMemoryUse::SDo(aValue.Vn(), aCache, GpReflectUtils_TotalMemoryUse::Mode::DONT_COUNT_SELF_SIZE);
        }
    } else
    {
        GpThrowCe<GpException>(fmt::format("Unsupported container {}", GpReflectType{type}));
    }

    return allocatedSize;
}

}// namespace GPlatform
