#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

//TODO replace with std:: implementation
//current pre SDT implementation https://github.com/josuttis/jthread
//https://medium.com/@vgasparyan1995/a-new-thread-in-c-20-jthread-ebd121ae8906
#include <jthread/source/jthread.hpp>

namespace GPlatform {

#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
	using GpThreadStopToken = std::stop_token;
#endif//#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
