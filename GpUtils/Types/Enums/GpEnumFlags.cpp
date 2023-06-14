#include "GpEnumFlags.hpp"
#include "../Strings/GpStringOps.hpp"

#if defined(GP_USE_ENUMS)

namespace GPlatform {

std::vector<std::u8string>  GpEnumFlags::ToStringArray (void) const
{
    std::vector<std::u8string_view> v_sv = ToStringViewArray();
    std::vector<std::u8string>      res;
    res.reserve(v_sv.size());

    for (std::u8string_view s: v_sv)
    {
        res.emplace_back(std::u8string(s));
    }

    return res;
}

std::vector<std::u8string_view> GpEnumFlags::ToStringViewArray (void) const
{
    std::vector<std::u8string_view> res;

    value_type value    = iValue;
    value_type id       = 0;

    while (value > 0)
    {
        if (value & 0b1)
        {
            res.emplace_back(ToStringFlag(id));
        }

        id++;
        value >>= 1;
    }

    return res;
}

void    GpEnumFlags::FromStringArray (const std::vector<std::u8string>& aArray)
{
    Clear();

    for (const std::u8string& s: aArray)
    {
        Set(s);
    }
}

void    GpEnumFlags::FromStringViewArray (const std::vector<std::u8string_view>& aArray)
{
    Clear();

    for (std::u8string_view s: aArray)
    {
        Set(s);
    }
}

std::u8string   GpEnumFlags::Echo (void) const
{
    std::vector<std::u8string_view> v = ToStringViewArray();
    return StrOps::SJoin<std::u8string_view>(v, u8","_sv);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_ENUMS)
