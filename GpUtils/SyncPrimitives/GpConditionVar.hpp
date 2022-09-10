#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/Units/SI/GpUnitsSI_Time.hpp"

#include <mutex>
#include <condition_variable>

namespace GPlatform {

class GP_UTILS_API GpConditionVar
{
public:
    enum class WakeUpState
    {
        WAIT,
        ONE,
        ALL
    };

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVar)
    CLASS_DECLARE_DEFAULTS(GpConditionVar)

    CLASS_TAG(THREAD_SAFE)

                                GpConditionVar  (void) noexcept = default;
                                ~GpConditionVar (void) noexcept = default;

    inline void                 WakeupAll       (void) noexcept;
    inline void                 WakeupOne       (void) noexcept;
    [[nodiscard]] inline bool   WaitForWakeup   (void) noexcept;
    [[nodiscard]] bool          WaitForWakeup   (const milliseconds_t aTimeout) noexcept;

private:
    mutable std::mutex          iWakeupMutex;
    std::condition_variable     iWakeupCV;
    WakeUpState                 iWakeUpState    = WakeUpState::WAIT;
    size_t                      iWaitCounter    = 0;
};

void    GpConditionVar::WakeupAll (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iWakeUpState = WakeUpState::ALL;
    }

    iWakeupCV.notify_all();
}

void    GpConditionVar::WakeupOne (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iWakeUpState = WakeUpState::ONE;
    }

    iWakeupCV.notify_one();
}

bool    GpConditionVar::WaitForWakeup (void) noexcept
{
    return WaitForWakeup(0.0_si_s);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
