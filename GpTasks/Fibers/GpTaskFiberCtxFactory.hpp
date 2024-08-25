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

    static void                         SInit                   (GpTaskFiberCtxFactory::SP aFactory);
    static void                         SClear                  (void);
    static GpTaskFiberCtxFactory&       S                       (void) noexcept {return sInstance.Vn();}

    virtual GpTaskFiberCtx::SP          NewInstance             (void) const = 0;

private:
    static GpTaskFiberCtxFactory::SP    sInstance;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
