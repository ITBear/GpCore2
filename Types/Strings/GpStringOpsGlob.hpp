#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include <string>
#include <string_view>

namespace GPlatform {

inline std::string operator+ (const std::string& aLeft, std::string_view aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, std::string_view aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, const std::string& aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, const char* aRight)
{
    std::string_view	right(aRight);
    std::string			res;
    res.reserve(aLeft.length() + right.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(right.data(), right.length());

    return res;
}

template<typename T>
inline std::string operator+ (std::string_view aLeft, T aRight)
{
    const std::string value = std::to_string(aRight);

    std::string res;
    res.reserve(aLeft.length() + value.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(value.data(), value.length());

    return res;
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
