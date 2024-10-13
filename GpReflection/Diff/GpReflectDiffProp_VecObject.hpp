#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecObject
{
public:
    CLASS_DD(GpReflectDiffProp_VecObject)

    using ContainerT                    = GpVectorReflectObjWrapBase;
    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

    struct DiffArrayHelpers
    {
        static size_t SizeInBytes (const typename ContainerT::value_type&       aValue,
                                   [[maybe_unused]] IsEqualCacheRefOptT&        aEqualCache,
                                   [[maybe_unused]] TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache,
                                   [[maybe_unused]] const size_t                aSrcElementStrideFrom,
                                   [[maybe_unused]] const size_t                aSrcElementStrideTo)
        {
            return GpReflectUtils_TotalMemoryUse::SDo(aValue, aTotalMemoryUseCache);
        }

        static bool Compare (const typename ContainerT::value_type&         aValueA,
                             const typename ContainerT::value_type&         aValueB,
                             [[maybe_unused]] IsEqualCacheRefOptT&          aEqualCache,
                             [[maybe_unused]] TotalMemoryUseCacheRefOptT&   aTotalMemoryUseCache,
                             [[maybe_unused]] const size_t                  aSrcElementStrideFrom,
                             [[maybe_unused]] const size_t                  aSrcElementStrideTo)
        {
            return GpReflectUtils_IsEqual::SDo(aValueA, aValueB, aEqualCache);
        }

        static const GpReflectObject& GetValAndIncPtrFrom (const GpReflectObject*&                      aPtr,
                                                           [[maybe_unused]] IsEqualCacheRefOptT&        aEqualCache,
                                                           [[maybe_unused]] TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache,
                                                           [[maybe_unused]] const size_t                aSrcElementStrideFrom,
                                                           [[maybe_unused]] const size_t                aSrcElementStrideTo)
        {
            const GpReflectObject& val = *aPtr;
            aPtr = reinterpret_cast<const GpReflectObject*>(reinterpret_cast<const char*>(aPtr) + aSrcElementStrideFrom);

            return val;
        }

        static const GpReflectObject& GetValAndIncPtrTo (const GpReflectObject*&                        aPtr,
                                                         [[maybe_unused]] IsEqualCacheRefOptT&          aEqualCache,
                                                         [[maybe_unused]] TotalMemoryUseCacheRefOptT&   aTotalMemoryUseCache,
                                                         [[maybe_unused]] const size_t                  aSrcElementStrideFrom,
                                                         [[maybe_unused]] const size_t                  aSrcElementStrideTo)
        {
            const GpReflectObject& val = *aPtr;
            aPtr = reinterpret_cast<const GpReflectObject*>(reinterpret_cast<const char*>(aPtr) + aSrcElementStrideTo);

            return val;
        }

        static GpReflectObject::SP Copy (const typename ContainerT::value_type& aValue)
        {
            return aValue.ReflectClone();
        }
    };

    using ArrayT = GpReflectDiffArray
    <
        ContainerT,
        GpReflectObject::SP,
        DiffArrayHelpers,
        IsEqualCacheRefOptT,
        TotalMemoryUseCacheRefOptT,
        const size_t,               // src element stride from
        const size_t                // src element stride to
    >;

public:
                            GpReflectDiffProp_VecObject     (void) noexcept = default;
    inline                  GpReflectDiffProp_VecObject     (const GpReflectDiffProp_VecObject& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecObject     (GpReflectDiffProp_VecObject&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecObject     (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecObject    (void) noexcept;

    static C::Opt::Val      SCheckAndMake                   (const ContainerT&              aValueFrom,
                                                             const ContainerT&              aValueTo,
                                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecObject::GpReflectDiffProp_VecObject (const GpReflectDiffProp_VecObject& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecObject::GpReflectDiffProp_VecObject (GpReflectDiffProp_VecObject&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecObject::GpReflectDiffProp_VecObject (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
