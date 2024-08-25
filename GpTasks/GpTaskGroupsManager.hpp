#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/GpTaskEnums.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

namespace GPlatform {

class GP_TASKS_API GpTaskGroupsManager
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskGroupsManager)
    CLASS_DD(GpTaskGroupsManager)
    TAG_SET(THREAD_SAFE)

    using TaskIdVecT                = boost::container::small_vector<GpTaskId, 32>;
    using TaskIdSetT                = boost::container::small_flat_set<GpTaskId, 8>;
    using GroupIdSetT               = boost::container::small_flat_set<GpTaskGroupId, 2>;

    using TaskIdsByGroupMapT        = boost::container::flat_map<GpTaskGroupId, TaskIdSetT>;
    using GroupIdsByTaskIdByMapT    = boost::container::flat_map<GpTaskId, GroupIdSetT>;

public:
                                GpTaskGroupsManager     (void) noexcept = default;
                                ~GpTaskGroupsManager    (void) noexcept = default;

    static GpTaskGroupsManager& S                       (void) noexcept {return sInstance;}

    inline bool                 AddTaskToGroup          (GpTaskId       aTaskId,
                                                         GpTaskGroupId  aGroupId);
    inline bool                 RemoveTaskFromGroup     (GpTaskId       aTaskId,
                                                         GpTaskGroupId  aGroupId);
    inline bool                 RemoveTaskFromAllGroups (GpTaskId       aTaskId);
    inline void                 FindTasksByGroupId      (GpTaskGroupId  aGroupId,
                                                         TaskIdVecT&    aTasksOut);

private:
    mutable GpSpinLockRW        iSpinLockRW;
    TaskIdsByGroupMapT          iTasksByGroups  GUARDED_BY(iSpinLockRW);
    GroupIdsByTaskIdByMapT      iGroupsByTasks  GUARDED_BY(iSpinLockRW);

    static GpTaskGroupsManager  sInstance;
};

bool    GpTaskGroupsManager::AddTaskToGroup
(
    const GpTaskId      aTaskId,
    const GpTaskGroupId aGroupId
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    return iTasksByGroups[aGroupId].emplace(aTaskId).second
        && iGroupsByTasks[aTaskId].emplace(aGroupId).second;
}

bool    GpTaskGroupsManager::RemoveTaskFromGroup
(
    const GpTaskId      aTaskId,
    const GpTaskGroupId aGroupId
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    // Erase task id from iTasksByGroups
    {
        auto groupIter = iTasksByGroups.find(aGroupId);

        if (groupIter == std::end(iTasksByGroups)) [[unlikely]]
        {
            return false;
        }

        auto& tasks = groupIter->second;

        if (tasks.erase(aTaskId) == 0) [[unlikely]]
        {
            return false;
        }

        if (tasks.empty())
        {
            iTasksByGroups.erase(groupIter);
        }
    }

    // Erase group id from iGroupsByTasks
    {
        auto taskIter = iGroupsByTasks.find(aTaskId);

        if (taskIter == std::end(iGroupsByTasks)) [[unlikely]]
        {
            return false;
        }

        auto& groups = taskIter->second;

        if (groups.erase(aGroupId) == 0) [[unlikely]]
        {
            return false;
        }

        if (groups.empty())
        {
            iGroupsByTasks.erase(taskIter);
        }
    }

    return true;
}

bool    GpTaskGroupsManager::RemoveTaskFromAllGroups (const GpTaskId aTaskId)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    auto taskIter = iGroupsByTasks.find(aTaskId);

    if (taskIter == std::end(iGroupsByTasks)) [[unlikely]]
    {
        return false;
    }

    auto& groups = taskIter->second;

    for (const GpTaskGroupId& grpupId: groups)
    {
        auto    groupIter       = iTasksByGroups.find(grpupId);
        auto&   groupsByTask    = groupIter->second;
        groupsByTask.erase(aTaskId);
        if (groupsByTask.empty())
        {
            iTasksByGroups.erase(groupIter);
        }
    }

    iGroupsByTasks.erase(taskIter);

    return true;
}

void    GpTaskGroupsManager::FindTasksByGroupId
(
    const GpTaskGroupId aGroupId,
    TaskIdVecT&         aTasksOut
)
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    auto groupIter = iTasksByGroups.find(aGroupId);

    if (groupIter == std::end(iTasksByGroups)) [[unlikely]]
    {
        return;
    }

    auto& tasks = groupIter->second;

    aTasksOut.reserve(std::size(tasks));
    aTasksOut.insert
    (
        std::end(aTasksOut),
        std::begin(tasks),
        std::end(tasks)
    );
}

}// namespace GPlatform
