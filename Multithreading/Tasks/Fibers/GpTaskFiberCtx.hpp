#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStack.hpp"
#include "../../Threads/GpThreadStopToken.hpp"
#include "../GpTask.hpp"
#include <cstddef>
#include <functional>

namespace GPlatform {

class GPCORE_API GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskFiberCtx)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberCtx)

    using FiberRunFnT   = std::function<void(GpThreadStopToken)>;

public:
                                GpTaskFiberCtx      (void) noexcept;
                                ~GpTaskFiberCtx     (void) noexcept;

    void                        Init                (void);
    void                        Clear               (void) noexcept;
    GpTask::ResT                Enter               (GpThreadStopToken  aStopToken,
                                                     GpTask::WP         aTask,
                                                     FiberRunFnT        aRunFn);
    static void                 SYeld               (const GpTask::ResT aRes);
    static GpTask::WP           SCurrentTask        (void);
    static bool                 SIsIntoFiber        (void) noexcept;

private:
    alignas(alignof(std::max_align_t)) std::array<std::byte, 32>    iFiberStorage;
    void*                                                           iFiberPtr = nullptr;
    GpTaskFiberStack::SP                                            iFiberStack;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
