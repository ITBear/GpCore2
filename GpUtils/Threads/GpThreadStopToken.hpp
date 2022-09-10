#pragma once

#include "../GpUtils_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <thread>

#if  __has_include(<stop_token>)
#   include <stop_token>
#elif  __has_include(<jthread/source/jthread.hpp>)
#   include <jthread/source/jthread.hpp>
#else
#   error Please add Jthread to include (https://github.com/josuttis/jthread.git)
#endif

namespace GPlatform {

#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
    using GpThreadStopToken     = std::stop_token;
    using GpThreadStopSource    = std::stop_source;
#endif//#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
