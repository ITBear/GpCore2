#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecObjectSP
{
public:
    CLASS_DD(GpReflectDiffProp_VecObjectSP)

    using ContainerT                    = std::vector<GpReflectObject::SP>;
    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

    struct DiffArrayHelpers
    {
        static size_t SizeInBytes (const typename ContainerT::value_type&       aValue,
                                   [[maybe_unused]] IsEqualCacheRefOptT&        aEqualCache,
                                   [[maybe_unused]] TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache)
        {
            if (aValue.IsNotNULL())
            {
                return GpReflectUtils_TotalMemoryUse::SDo(aValue.Vn(), aTotalMemoryUseCache);
            } else
            {
                return sizeof(GpReflectObject::SP);
            }
        }

        static bool Compare (const typename ContainerT::value_type&         aValueA,
                             const typename ContainerT::value_type&         aValueB,
                             [[maybe_unused]] IsEqualCacheRefOptT&          aEqualCache,
                             [[maybe_unused]] TotalMemoryUseCacheRefOptT&   aTotalMemoryUseCache)
        {
            if (aValueA.IsNotNULL())
            {
                if (aValueB.IsNotNULL())
                {
                    return GpReflectUtils_IsEqual::SDo(aValueA.Vn(), aValueB.Vn(), aEqualCache);
                } else
                {
                    return false;
                }
            } else
            {
                if (aValueB.IsNotNULL())
                {
                    return false;
                } else
                {
                    return true;
                }
            }
        }

        static const GpReflectObject::SP& GetValAndIncPtrFrom (const GpReflectObject::SP*&                  aPtr,
                                                               [[maybe_unused]] IsEqualCacheRefOptT&        aEqualCache,
                                                               [[maybe_unused]] TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache)
        {
            return *aPtr++;
        }

        static const GpReflectObject::SP& GetValAndIncPtrTo (const GpReflectObject::SP*&                    aPtr,
                                                             [[maybe_unused]] IsEqualCacheRefOptT&          aEqualCache,
                                                             [[maybe_unused]] TotalMemoryUseCacheRefOptT&   aTotalMemoryUseCache)
        {
            return *aPtr++;
        }

        static GpReflectObject::SP Copy (const typename ContainerT::value_type& aValue)
        {
            if (aValue.IsNotNULL())
            {
                return aValue.Vn().ReflectClone();
            } else
            {
                return GpReflectObject::SP::SNull();
            }
        }
    };

    using ArrayT = GpReflectDiffArray
    <
        ContainerT,
        GpReflectObject::SP,
        DiffArrayHelpers,
        IsEqualCacheRefOptT,
        TotalMemoryUseCacheRefOptT
    >;

public:
                            GpReflectDiffProp_VecObjectSP   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecObjectSP   (const GpReflectDiffProp_VecObjectSP& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecObjectSP   (GpReflectDiffProp_VecObjectSP&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecObjectSP   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecObjectSP  (void) noexcept;

    static C::Opt::Val      SCheckAndMake                   (const ContainerT&              aValueFrom,
                                                             const ContainerT&              aValueTo,
                                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecObjectSP::GpReflectDiffProp_VecObjectSP (const GpReflectDiffProp_VecObjectSP& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecObjectSP::GpReflectDiffProp_VecObjectSP (GpReflectDiffProp_VecObjectSP&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecObjectSP::GpReflectDiffProp_VecObjectSP (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
