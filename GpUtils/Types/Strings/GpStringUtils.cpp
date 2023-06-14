#include "GpStringUtils.hpp"
#include "GpUTF.hpp"

#if defined(GP_USE_STRINGS)

#include <iostream>

namespace GPlatform {

void    GpStringUtils::SCerr (std::string_view aStr)
{
    std::cerr << aStr;
    std::cerr.flush();
}

void    GpStringUtils::SCerr (std::u8string_view aStr)
{
    std::cerr << GpUTF::S_UTF8_To_STR(aStr);
    std::cerr.flush();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_STRINGS)
