#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../Types/Containers/GpContainersT.hpp"
#include "../../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

#include <mutex>
#include <condition_variable>

namespace GPlatform {

class GPCORE_API GpConditionVar
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVar)
    CLASS_DECLARE_DEFAULTS(GpConditionVar)

    CLASS_TAG(THREAD_SAFE)

                            GpConditionVar  (void) noexcept;
                            ~GpConditionVar (void) noexcept;

    void                    WakeupAll       (void) noexcept;
    void                    WakeupOne       (void) noexcept;
    void                    WaitForWakeup   (void) noexcept;
    void                    WaitForWakeup   (const milliseconds_t aTimeout) noexcept;

private:
    mutable std::mutex      iWakeupMutex;
    std::condition_variable iWakeupCV;
    count_t                 iWaitCounter    = 0_cnt;
    bool                    iNeedToWakeUp   = false;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
