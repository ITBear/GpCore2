#include <GpCore2/GpTasks/Fibers/Boost/GpStackImplPoolBoost.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

namespace GPlatform {

GpStackImplPoolBoost    GpStackImplPoolBoost::sInstance;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
