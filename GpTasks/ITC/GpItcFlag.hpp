#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <mutex>
#include <shared_mutex>
#include "GpItcResult.hpp"

namespace GPlatform {

class GP_TASKS_API GpItcFlag
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcFlag)
    CLASS_DD(GpItcFlag)

public:
                        GpItcFlag       (void) noexcept = default;
                        ~GpItcFlag      (void) noexcept = default;

    inline void         Reset           (void) noexcept;
    inline bool         Test            (void) const noexcept;
    inline bool         TestAndSet      (void) noexcept;
    inline bool         TestAndSet      (GpItcResult::SP                        aValue,
                                         std::function<void(GpItcResult::SP&&)> aOnSetFn) noexcept;
    inline void         Wait            (void);
    [[nodiscard]] bool  TryWait         (const milliseconds_t aWaitTimeout);

    template<typename LockT>
    [[nodiscard]] bool  TryWait         (LockT&                 aLock,
                                         const milliseconds_t   aWaitTimeout);

private:
    void                ProcessSet      (void) noexcept;
    void                ProcessSet      (GpItcResult::SP                        aValue,
                                         std::function<void(GpItcResult::SP&&)> aOnSetFn) noexcept;

protected:
    mutable GpSpinlock  iLock;
    std::vector<GpUUID> iTasks;
    GpConditionVar::SP  iConditionVar;
    bool                iFlag = false;
};

void    GpItcFlag::Reset (void) noexcept
{
    std::scoped_lock lock(iLock);
    iFlag = false;
}

bool    GpItcFlag::Test (void) const noexcept
{
    std::scoped_lock lock(iLock);
    return iFlag;
}

bool    GpItcFlag::TestAndSet (void) noexcept
{
    std::scoped_lock lock(iLock);

    if (iFlag)
    {
        return true;
    }

    iFlag = true;

    ProcessSet();

    return false;
}

bool    GpItcFlag::TestAndSet
(
    GpItcResult::SP                         aValue,
    std::function<void(GpItcResult::SP&&)>  aOnSetFn
) noexcept
{
    std::scoped_lock lock(iLock);

    if (iFlag)
    {
        return true;
    }

    iFlag = true;

    ProcessSet(std::move(aValue), aOnSetFn);

    return false;
}

void    GpItcFlag::Wait (void)
{
    std::ignore = TryWait(-1.0_si_s);
}

template<typename LockT>
bool    GpItcFlag::TryWait
(
    LockT&                  aLock,
    const milliseconds_t    aWaitTimeout
)
{
    GpUnlockGuard unlock(aLock);
    return TryWait(aWaitTimeout);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
