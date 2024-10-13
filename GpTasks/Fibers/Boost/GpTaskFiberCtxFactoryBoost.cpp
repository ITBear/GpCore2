#include <GpCore2/GpTasks/Fibers/Boost/GpTaskFiberCtxFactoryBoost.hpp>
#include <GpCore2/GpTasks/Fibers/Boost/GpTaskFiberCtxBoost.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

namespace GPlatform {

GpTaskFiberCtx::SP  GpTaskFiberCtxFactoryBoost::NewInstance (void) const
{
    return MakeSP<GpTaskFiberCtxBoost>();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
