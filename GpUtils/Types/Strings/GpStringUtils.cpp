#include "GpStringUtils.hpp"
#include "GpUTF.hpp"

#if defined(GP_USE_STRINGS)

#include <iostream>

namespace GPlatform {

void    GpStringUtils::SCout (std::string_view aStr) noexcept
{
    std::cout << std::endl << aStr;
    std::cout.flush();
}

void    GpStringUtils::SCerr (std::string_view aStr) noexcept
{
    std::cerr << std::endl << aStr;
    std::cerr.flush();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
