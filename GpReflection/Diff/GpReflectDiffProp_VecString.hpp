#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecString
{
public:
    CLASS_DD(GpReflectDiffProp_VecString)

    using ContainerT = std::vector<std::string>;

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
                            GpReflectDiffProp_VecString     (void) noexcept = default;
    inline                  GpReflectDiffProp_VecString     (const GpReflectDiffProp_VecString& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecString     (GpReflectDiffProp_VecString&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecString     (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecString    (void) noexcept;

    static C::Opt::Val      SCheckAndMake                   (const ContainerT& aValueFrom,
                                                             const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecString::GpReflectDiffProp_VecString (const GpReflectDiffProp_VecString& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecString::GpReflectDiffProp_VecString (GpReflectDiffProp_VecString&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecString::GpReflectDiffProp_VecString (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
