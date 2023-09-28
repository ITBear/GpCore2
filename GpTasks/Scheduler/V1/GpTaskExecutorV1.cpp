#include "GpTaskExecutorV1.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../../../GpService/GpService.hpp"

namespace GPlatform {

GpTaskExecutorV1::GpTaskExecutorV1
(
    const size_t        aId,
    ConsumerT&&         aTasksConsumer,
    GpTaskScheduler&    aTasksScheduler,
    DonePromiseT&&      aDonePromise
) noexcept:
iId            (aId),
iTasksConsumer (std::move(aTasksConsumer)),
iTasksScheduler(aTasksScheduler),
iDonePromise   (std::move(aDonePromise))
{
}

GpTaskExecutorV1::~GpTaskExecutorV1 (void) noexcept
{
}

void    GpTaskExecutorV1::Run (std::atomic_flag& aStopRequest) noexcept
{
    std::u8string exMsg;

    try
    {
        // Repeat until stop requested
        while (!aStopRequest.test())
        {
            // Consume next task
            typename ConsumerT::ItcResultT::C::Opt::Val nextTaskItcResOpt = iTasksConsumer.Consume(500.0_si_ms);

            // No task
            if (!nextTaskItcResOpt.has_value())
            {
                continue;
            }

            // Get consume result value
            GpItcResult<GpTask::SP>& nextTaskItcRes = nextTaskItcResOpt.value();

            // Check if result is payload (actualy a task)
            if (!nextTaskItcRes.IsPayload())
            {
                continue;
            }

            // Extract task
            GpTask::SP  taskSP  = std::move(nextTaskItcRes.Payload());
            GpTask&     task    = taskSP.V();

            // Run task
            const GpTaskRunRes::EnumT taskRes = task.Execute();

            // Reschedule task
            if (iTasksScheduler.Reschedule(taskRes, std::move(taskSP)) == false)
            {
                THROW_GP(u8"Failed to Reschedule"_sv);
            }
        }

        iDonePromise.Fulfill(ssize_t(1));

        return;
    } catch (const GpException& e)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8", exception: "_sv + e.what();
    } catch (const std::exception& e)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8", exception: "_sv + e.what();
    } catch (...)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8",  unknown exception"_sv;
    }

    GpStringUtils::SCerr(exMsg);
    iDonePromise.Fulfill(GpException(exMsg));

    // Stop service
    GpService::SRequestStop();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
