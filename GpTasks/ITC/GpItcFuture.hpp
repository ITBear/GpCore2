#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <mutex>
#include <shared_mutex>

#include "GpItcFlag.hpp"

namespace GPlatform {

class GpItcPromise;

class GP_TASKS_API GpItcFuture
{
    friend class GpItcPromise;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcFuture)
    CLASS_DD(GpItcFuture)

public:
                        GpItcFuture         (void) noexcept = default;
                        ~GpItcFuture        (void) noexcept = default;

    bool                IsReady             (void) const noexcept {return iItcFlag.Test();}
    void                Wait                (void) {iItcFlag.Wait();}
    GpItcResult::SP     Result              (void) noexcept {return iResult;}

private:
    bool                SetResult           (GpItcResult::SP aResult) noexcept;
    void                PromiseDestructed   (void) noexcept {iItcFlag.TestAndSet();}

private:
    GpItcResult::SP     iResult;
    GpItcFlag           iItcFlag;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
