#pragma once

#include <typeinfo>
#include <cstring>

namespace GPlatform {

class GpTypeInfoUtils
{
public:
    static inline bool          SIsSame (const std::type_info&  aTypeA,
                                         const std::type_info&  aTypeB) noexcept;
};

bool    GpTypeInfoUtils::SIsSame
(
    const std::type_info&   aTypeA,
    const std::type_info&   aTypeB
) noexcept
{
    const size_t typeHashCodeA = aTypeA.hash_code();
    const size_t typeHashCodeB = aTypeB.hash_code();

    return     (typeHashCodeA == typeHashCodeB)
            || (std::strcmp(aTypeA.name(), aTypeB.name()) == 0);
}

}//namespace GPlatform
