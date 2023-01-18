#include "GpStringUtils.hpp"

#if defined(GP_USE_STRINGS)

#include <iostream>

namespace GPlatform {

void    GpStringUtils::SCerr (std::string_view aStr)
{
    std::cerr << aStr;
    std::cerr.flush();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_STRINGS)
