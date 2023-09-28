#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

thread_local GpTask::C::Opt::Ref    __GpTask__thread_current_task;
std::atomic<GpTask::IdT>            GpTask::sIdCounter = GpTask::IdT(1);

GpTask::~GpTask (void) noexcept
{
    const auto taskId = GpTask::IdT();

    try
    {
        GpTaskVarStorage::S().RemoveTask(taskId);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask unknown exception"_sv);
    }

    try
    {
        GpTaskPayloadStorage::S().RemoveTask(taskId);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskPayloadStorage::RemoveTask exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskPayloadStorage::RemoveTask exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpTask::~GpTask]: GpTaskPayloadStorage::RemoveTask unknown exception"_sv);
    }

    StartPromiseHolder().Fulfill();
    DonePromiseHolder().Fulfill();
}

GpTask::C::Opt::Ref GpTask::SCurrentTask (void) noexcept
{
    return __GpTask__thread_current_task;
}

GpTaskRunRes::EnumT GpTask::Execute (void) noexcept
{
    __GpTask__thread_current_task = *this;
    const GpTaskRunRes::EnumT runRes = Run();
    __GpTask__thread_current_task.reset();

    return runRes;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
