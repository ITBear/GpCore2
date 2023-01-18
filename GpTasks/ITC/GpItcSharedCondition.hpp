#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <mutex>
#include <shared_mutex>

namespace GPlatform {

class GP_TASKS_API GpItcSharedCondition
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcSharedCondition)
    CLASS_DD(GpItcSharedCondition)

    enum class ActionCondNotMetRes
    {
        CONTINUE,
        BREAK
    };

    using ConditionFnT          = std::function<bool()>;
    using ActionCondMetFnT      = std::function<void()>;
    using ActionCondNotMetFnT   = std::function<ActionCondNotMetRes()>;
    using WaitForConditionResT  = GpConditionVar::WaitForResT;

public:
                                GpItcSharedCondition    (void) noexcept = default;
                                ~GpItcSharedCondition   (void) noexcept = default;

    inline void                 Do                      (std::function<void()> aFn) const;
    void                        WakeupOne               (std::function<void()> aFn);
    void                        WakeupAll               (std::function<void()> aFn);
    [[nodiscard]] inline WaitForConditionResT
                                WaitForCondition        (const milliseconds_t   aTimeout,
                                                         ConditionFnT           aConditionFn,
                                                         ActionCondMetFnT       aOnConditionMetFn,
                                                         ActionCondNotMetFnT    aOnConditionNotMetFn);

    inline void                 AddTaskGuid             (const GpUUID& aTaskGuid);
    inline void                 RemoveTaskGuid          (const GpUUID& aTaskGuid);

private:
    [[nodiscard]] WaitForConditionResT
                                _WaitForCondition       (const milliseconds_t   aTimeout,
                                                         ConditionFnT           aConditionFn,
                                                         ActionCondMetFnT       aOnConditionMetFn,
                                                         ActionCondNotMetFnT    aOnConditionNotMetFn);
    void                        _AddTaskGuid            (const GpUUID& aTaskGuid);
    void                        _RemoveTaskGuid         (const GpUUID& aTaskGuid);

protected:
    std::vector<GpUUID>         iTaskFiberGuids;
    GpConditionVar              iCV;
};

void    GpItcSharedCondition::Do (std::function<void()> aFn) const
{
    iCV.Do(aFn);
}

GpItcSharedCondition::WaitForConditionResT  GpItcSharedCondition::WaitForCondition
(
    const milliseconds_t    aTimeout,
    ConditionFnT            aConditionFn,
    ActionCondMetFnT        aOnConditionMetFn,
    ActionCondNotMetFnT     aOnConditionNotMetFn
)
{
    //Fast check condition
    bool fastRes = false;
    iCV.Do
    (
        [&]()
        {
            fastRes = aConditionFn();

            if (fastRes)
            {
                aOnConditionMetFn();
            } else  if (aTimeout <= 0.0_si_s)
            {
                aOnConditionNotMetFn();
            }
        }
    );

    if (fastRes)
    {
        return WaitForConditionResT::OK;
    } else if (aTimeout <= 0.0_si_s)
    {
        return WaitForConditionResT::TIMEOUT;
    }

    return _WaitForCondition
    (
        aTimeout,
        aConditionFn,
        aOnConditionMetFn,
        aOnConditionNotMetFn
    );
}

void    GpItcSharedCondition::AddTaskGuid (const GpUUID& aTaskGuid)
{
    iCV.Do
    (
        [&]()
        {
            _AddTaskGuid(aTaskGuid);
        }
    );
}

void    GpItcSharedCondition::RemoveTaskGuid (const GpUUID& aTaskGuid)
{
    iCV.Do
    (
        [&]()
        {
            _RemoveTaskGuid(aTaskGuid);
        }
    );
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
