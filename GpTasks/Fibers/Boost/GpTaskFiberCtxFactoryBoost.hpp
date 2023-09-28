#pragma once

#include "../GpTaskFiberCtxFactory.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

namespace GPlatform {

class GP_TASKS_API GpTaskFiberCtxFactoryBoost final: public GpTaskFiberCtxFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberCtxFactoryBoost)
    CLASS_DD(GpTaskFiberCtxFactoryBoost)
    TAG_SET(THREAD_SAFE)

public:
                                    GpTaskFiberCtxFactoryBoost  (void) noexcept = default;
    virtual                         ~GpTaskFiberCtxFactoryBoost (void) noexcept override final = default;

    virtual GpTaskFiberCtx::SP      NewInstance                 (void) const override final;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
