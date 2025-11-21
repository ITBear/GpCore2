#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtx.hpp>

namespace GPlatform {

class GP_TASKS_API GpTaskFiberCtxFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberCtxFactory)
    CLASS_DD(GpTaskFiberCtxFactory)
    TAG_SET(THREAD_SAFE)

protected:
                                        GpTaskFiberCtxFactory   (void) noexcept = default;

public:
    virtual                             ~GpTaskFiberCtxFactory  (void) noexcept = default;

    static void                         SInit                   (GpTaskFiberCtxFactory::UP aFactoryUP);
    static void                         SClear                  (void);
    static const GpTaskFiberCtxFactory& S                       (void) noexcept {return *sInstance;}

    virtual GpTaskFiberCtx::UP          NewInstance             (void) const = 0;

private:
    static GpTaskFiberCtxFactory::UP    sInstance;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
