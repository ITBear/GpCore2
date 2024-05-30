#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecBLOB
{
public:
    CLASS_DD(GpReflectDiffProp_VecBLOB)

    using ContainerT = std::vector<GpBytesArray>;

    struct DiffArrayHelpers
    {
        using value_type = typename ContainerT::value_type;

        static size_t SizeInBytes (const value_type& aValue)
        {
            return std::size(aValue) * sizeof(typename ContainerT::value_type::value_type);
        }

        static bool Compare (const value_type& aValueA,
                             const value_type& aValueB)
        {
            return aValueA == aValueB;
        }

        static const value_type& GetValAndIncPtrFrom (const value_type*& aPtr)
        {
            return *aPtr++;
        }

        static const value_type& GetValAndIncPtrTo  (const value_type*& aPtr)
        {
            return *aPtr++;
        }

        static value_type Copy (const value_type& aValue)
        {
            return aValue;
        }
    };

    using ArrayT = GpReflectDiffArray
    <
        ContainerT,
        typename ContainerT::value_type,
        DiffArrayHelpers
    >;

public:
                            GpReflectDiffProp_VecBLOB   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecBLOB   (const GpReflectDiffProp_VecBLOB& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecBLOB   (GpReflectDiffProp_VecBLOB&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecBLOB   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecBLOB  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecBLOB::GpReflectDiffProp_VecBLOB (const GpReflectDiffProp_VecBLOB& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecBLOB::GpReflectDiffProp_VecBLOB (GpReflectDiffProp_VecBLOB&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecBLOB::GpReflectDiffProp_VecBLOB (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
