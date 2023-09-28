#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <mutex>
#include <shared_mutex>

#include "GpTasks_global.hpp"
#include "../GpUtils/Macro/GpMacroClass.hpp"
#include "../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../GpUtils/SyncPrimitives/GpSpinlock.hpp"
#include "../GpUtils/Types/Containers/GpAny.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskPayloadStorage
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskPayloadStorage)
    CLASS_DD(GpTaskPayloadStorage)

    using TaskIdT           = u_int_64;
    using TaskMapPayloadT   = std::deque<GpAny>;
    using ContainerT        = std::unordered_map<TaskIdT, TaskMapPayloadT>;
    using AnyOptT           = std::optional<GpAny>;

public:
                                    GpTaskPayloadStorage    (void) noexcept = default;
                                    ~GpTaskPayloadStorage   (void) noexcept = default;

    static GpTaskPayloadStorage&    S                       (void) noexcept {return sInstance;}

    inline void                     RemoveTask              (const TaskIdT  aTaskId);

    inline void                     PushPayload             (const TaskIdT  aTaskId,
                                                             GpAny&&        aPayload);
    inline AnyOptT                  PopPayload              (const TaskIdT  aTaskId);

private:
    mutable GpSpinlock              iLock;
    ContainerT                      iPayload;
    static GpTaskPayloadStorage     sInstance;
};

void    GpTaskPayloadStorage::RemoveTask (const TaskIdT aTaskId)
{
    std::scoped_lock lock(iLock);
    iPayload.erase(aTaskId);
}

void    GpTaskPayloadStorage::PushPayload
(
    const TaskIdT   aTaskId,
    GpAny&&         aPayload
)
{
    std::scoped_lock lock(iLock);
    iPayload[aTaskId].push_back(std::move(aPayload));
}

GpTaskPayloadStorage::AnyOptT   GpTaskPayloadStorage::PopPayload (const TaskIdT aTaskId)
{
    std::scoped_lock lock(iLock);

    auto taskIter = iPayload.find(aTaskId);

    if (taskIter == iPayload.end())
    {
        return std::nullopt;
    }

    TaskMapPayloadT& taskPayload = taskIter->second;

    if (!taskPayload.empty())
    {
        AnyOptT res = std::move(taskPayload.front());
        taskPayload.pop_front();
        return res;
    } else
    {
        return std::nullopt;
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
