#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <mutex>
#include <shared_mutex>

#include "GpTasks_global.hpp"
#include "../GpUtils/Macro/GpMacroClass.hpp"
#include "../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../GpUtils/SyncPrimitives/GpRWSpinLock.hpp"
#include "../GpUtils/Types/Containers/GpAny.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskVarStorage
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskVarStorage)
    CLASS_DD(GpTaskVarStorage)

    using TaskIdT           = u_int_64;
    using TaskMapValuesT    = std::map<std::u8string, GpAny, std::less<>>;
    using ContainerT        = std::unordered_map<TaskIdT, TaskMapValuesT>;
    using AnyOptT           = std::optional<GpAny>;
    using AnyOptCRefT       = std::optional<std::reference_wrapper<const GpAny>>;

public:
                                    GpTaskVarStorage    (void) noexcept = default;
                                    ~GpTaskVarStorage   (void) noexcept = default;

    static GpTaskVarStorage&        S                   (void) noexcept {return sInstance;}

    inline void                     RemoveTask          (const TaskIdT      aTaskId);

    inline void                     SetVar              (const TaskIdT      aTaskId,
                                                         std::u8string      aKey,
                                                         GpAny&&            aValue);
    inline AnyOptT                  GetVarCopy          (const TaskIdT      aTaskId,
                                                         std::u8string_view aKey) const;
    inline AnyOptCRefT              GetVarRef           (const TaskIdT      aTaskId,
                                                         std::u8string_view aKey) const;

private:
    mutable GpRWSpinLock            iLock;
    ContainerT                      iVars;
    static GpTaskVarStorage         sInstance;
};

void    GpTaskVarStorage::RemoveTask (const TaskIdT aTaskId)
{
    std::scoped_lock lock(iLock);
    iVars.erase(aTaskId);
}

void    GpTaskVarStorage::SetVar
(
    const TaskIdT   aTaskId,
    std::u8string   aKey,
    GpAny&&         aValue
)
{
    std::scoped_lock lock(iLock);

    auto[iter, isInserted] = iVars[aTaskId].try_emplace(std::move(aKey), std::move(aValue));

    if (!isInserted)
    {
        iter->second = std::move(aValue);
    }
}

GpTaskVarStorage::AnyOptT   GpTaskVarStorage::GetVarCopy
(
    const TaskIdT       aTaskId,
    std::u8string_view  aKey
) const
{
    std::shared_lock lock(iLock);

    const auto taskIter = iVars.find(aTaskId);

    if (taskIter == iVars.end())
    {
        return std::nullopt;
    }

    const auto& taskVars    = taskIter->second;
    const auto  taskVarIter = taskVars.find(aKey);

    if (taskVarIter == taskVars.end())
    {
        return std::nullopt;
    }

    return taskVarIter->second;
}

GpTaskVarStorage::AnyOptCRefT   GpTaskVarStorage::GetVarRef
(
    const TaskIdT       aTaskId,
    std::u8string_view  aKey
) const
{
    std::shared_lock lock(iLock);

    const auto taskIter = iVars.find(aTaskId);

    if (taskIter == iVars.end())
    {
        return std::nullopt;
    }

    const auto& taskVars    = taskIter->second;
    const auto  taskVarIter = taskVars.find(aKey);

    if (taskVarIter == taskVars.end())
    {
        return std::nullopt;
    }

    return taskVarIter->second;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
