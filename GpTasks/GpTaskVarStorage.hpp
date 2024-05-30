#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpTasks_global.hpp"
#include "GpTaskEnums.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>

#include <mutex>
#include <shared_mutex>

namespace GPlatform {

class GP_TASKS_API GpTaskVarStorage
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskVarStorage)
    CLASS_DD(GpTaskVarStorage)

    using TaskMapValuesT    = std::map<std::string, GpAny, std::less<>>;
    using ContainerT        = std::map<GpTaskId, TaskMapValuesT>;
    using AnyOptT           = std::optional<GpAny>;
    using AnyOptCRefT       = std::optional<std::reference_wrapper<const GpAny>>;

public:
                                GpTaskVarStorage    (void) noexcept = default;
                                ~GpTaskVarStorage   (void) noexcept = default;

    static GpTaskVarStorage&    S                   (void) noexcept {return sInstance;}

    inline void                 RemoveTask          (GpTaskId       aTaskId);

    inline void                 SetVar              (GpTaskId       aTaskId,
                                                     std::string    aKey,
                                                     GpAny&&        aValue);
    inline AnyOptT              GetVarCopy          (GpTaskId           aTaskId,
                                                     std::string_view   aKey) const;
    inline AnyOptCRefT          GetVarRef           (GpTaskId           aTaskId,
                                                     std::string_view   aKey) const;

private:
    mutable GpSpinLockRW        iSpinLockRW;
    ContainerT                  iVars GUARDED_BY(iSpinLockRW);

    static GpTaskVarStorage     sInstance;
};

void    GpTaskVarStorage::RemoveTask (const GpTaskId aTaskId)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iVars.erase(aTaskId);
}

void    GpTaskVarStorage::SetVar
(
    const GpTaskId  aTaskId,
    std::string     aKey,
    GpAny&&         aValue
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iVars[aTaskId].insert_or_assign(std::move(aKey), std::move(aValue));
}

GpTaskVarStorage::AnyOptT   GpTaskVarStorage::GetVarCopy
(
    const GpTaskId      aTaskId,
    std::string_view    aKey
) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    const auto taskIter = iVars.find(aTaskId);

    if (taskIter == std::end(iVars))
    {
        return std::nullopt;
    }

    const auto& taskVars    = taskIter->second;
    const auto  taskVarIter = taskVars.find(aKey);

    if (taskVarIter == std::end(taskVars))
    {
        return std::nullopt;
    }

    return taskVarIter->second;
}

GpTaskVarStorage::AnyOptCRefT   GpTaskVarStorage::GetVarRef
(
    const GpTaskId      aTaskId,
    std::string_view    aKey
) const
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    const auto taskIter = iVars.find(aTaskId);

    if (taskIter == std::end(iVars))
    {
        return std::nullopt;
    }

    const auto& taskVars    = taskIter->second;
    const auto  taskVarIter = taskVars.find(aKey);

    if (taskVarIter == std::end(taskVars))
    {
        return std::nullopt;
    }

    return taskVarIter->second;
}

}// namespace GPlatform
