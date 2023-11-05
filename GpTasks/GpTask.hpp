#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskEnums.hpp"
#include "GpTaskVarStorage.hpp"
#include "GpTaskPayloadStorage.hpp"
#include "ITC/GpItcSharedPromise.hpp"
#include "../GpUtils/Macro/GpMacroTags.hpp"

namespace GPlatform {

class GP_TASKS_API GpTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTask)
    CLASS_DD(GpTask)
    TAG_SET(THREAD_SAFE)

    using StartPromiseT = GpItcSharedPromise<GpAny>;
    using StartFutureT  = typename StartPromiseT::FutureT;

    using DonePromiseT  = GpItcSharedPromise<GpAny>;
    using DoneFutureT   = typename DonePromiseT::FutureT;

protected:
    inline                                  GpTask              (std::u8string              aName,
                                                                 const GpTaskMode::EnumT    aTaskMode) noexcept;
    inline                                  GpTask              (const GpTaskMode::EnumT    aTaskMode) noexcept;

public:
    virtual                                 ~GpTask             (void) noexcept;

    static GpTask::C::Opt::Ref              SCurrentTask        (void) noexcept;

    inline std::u8string_view               Name                (void) const noexcept;
    inline GpTaskId                         Id                  (void) const noexcept;
    inline GpUUID                           IdAsUUID            (void) const noexcept;
    inline GpTaskMode::EnumT                Mode                (void) const noexcept;

    // Payload (Scheduler::MakeTaskReady)
    inline void                             PushPayload         (GpAny aPayload);
    inline GpTaskPayloadStorage::AnyOptT    PopPayload          (void);

    // Vars
    inline void                             SetVar              (std::u8string  aKey,
                                                                 GpAny          aValue);
    inline GpTaskVarStorage::AnyOptT        GetVarCopy          (std::u8string_view aKey) const;
    inline GpTaskVarStorage::AnyOptCRefT    GetVarRef           (std::u8string_view aKey) const;

    // Use only from Task Executor or Task Scheduler
    GpTaskRunRes::EnumT                     Execute             (void) noexcept;
    inline bool                             IsStopRequested     (void) const noexcept;
    inline void                             UpStopRequestFlag   (void) noexcept;

    // Task Start/Done promise
    inline StartFutureT::SP                 GetStartFuture      (void);
    inline DoneFutureT::SP                  GetDoneFuture       (void);

protected:
    virtual GpTaskRunRes::EnumT             Run                 (void) noexcept = 0;

    inline StartPromiseT&                   StartPromise        (void) noexcept;
    inline DonePromiseT&                    DonePromise         (void) noexcept;

private:
    inline static GpTaskId                  SNextId             (void) noexcept;

private:
    const std::u8string                     iName;
    const GpTaskId                          iId;
    const GpTaskMode::EnumT                 iMode;
    std::atomic_bool                        iIsStopRequested    = false;
    StartPromiseT                           iStartPromise;
    DonePromiseT                            iDonePromise;

    static std::atomic<GpTaskId>            sIdCounter;
};

GpTask::GpTask
(
    std::u8string           aName,
    const GpTaskMode::EnumT aTaskMode
) noexcept:
iName(std::move(aName)),
iId  (SNextId()),
iMode(aTaskMode)
{
}

GpTask::GpTask (const GpTaskMode::EnumT aTaskMode) noexcept:
iId  (SNextId()),
iMode(aTaskMode)
{
}

std::u8string_view  GpTask::Name (void) const noexcept
{
    return iName;
}

GpTaskId    GpTask::Id (void) const noexcept
{
    return iId;
}

GpUUID  GpTask::IdAsUUID (void) const noexcept
{
    GpUUID::DataT uuid;

    const u_int_64 id = Id();

    std::memcpy(uuid.data() + 0, "GpTask::", 8);
    std::memcpy(uuid.data() + 8, &id, 8);

    return GpUUID(uuid);
}

GpTaskMode::EnumT   GpTask::Mode (void) const noexcept
{
    return iMode;
}

void    GpTask::PushPayload (GpAny aPayload)
{
    GpTaskPayloadStorage::S().PushPayload
    (
        Id(),
        std::move(aPayload)
    );
}

GpTaskPayloadStorage::AnyOptT   GpTask::PopPayload (void)
{
    return GpTaskPayloadStorage::S().PopPayload(Id());
}

void    GpTask::SetVar
(
    std::u8string   aKey,
    GpAny           aValue
)
{
    GpTaskVarStorage::S().SetVar
    (
        Id(),
        std::move(aKey),
        std::move(aValue)
    );
}

GpTaskVarStorage::AnyOptT   GpTask::GetVarCopy (std::u8string_view aKey) const
{
    return GpTaskVarStorage::S().GetVarCopy(Id(), aKey);
}

GpTaskVarStorage::AnyOptCRefT   GpTask::GetVarRef (std::u8string_view aKey) const
{
    return GpTaskVarStorage::S().GetVarRef(Id(), aKey);
}

bool    GpTask::IsStopRequested (void) const noexcept
{
    return iIsStopRequested.load(std::memory_order_acquire);
}

void    GpTask::UpStopRequestFlag (void) noexcept
{
    iIsStopRequested.store(true, std::memory_order_release);
}

GpTask::StartFutureT::SP    GpTask::GetStartFuture (void)
{
    return iStartPromise.Future();
}

GpTask::DoneFutureT::SP GpTask::GetDoneFuture (void)
{
    return iDonePromise.Future();
}

GpTask::StartPromiseT&  GpTask::StartPromise (void) noexcept
{
    return iStartPromise;
}

GpTask::DonePromiseT&   GpTask::DonePromise (void) noexcept
{
    return iDonePromise;
}

GpTaskId    GpTask::SNextId (void) noexcept
{
    return sIdCounter.fetch_add(1, std::memory_order_relaxed);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
