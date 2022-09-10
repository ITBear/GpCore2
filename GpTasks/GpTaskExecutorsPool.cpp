#include "GpTaskExecutorsPool.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskAccessor.hpp"

namespace GPlatform {

GpTaskExecutorsPool::GpTaskExecutorsPool (void) noexcept
{
}

GpTaskExecutorsPool::~GpTaskExecutorsPool (void) noexcept
{
}

void    GpTaskExecutorsPool::RequestStop (void) noexcept
{
    std::scoped_lock lock(iLock);

    for (GpThread& thread: iThreads)
    {
        thread.RequestStop();
    }
}

void    GpTaskExecutorsPool::Join (void) noexcept
{
    std::scoped_lock lock(iLock);

    for (GpThread& thread: iThreads)
    {
        thread.Join();
    }
}

void    GpTaskExecutorsPool::PreInit (const size_t aCount)
{
    iExecutors.reserve(aCount);
    iThreads.reserve(aCount);

    // Create threads
    for (size_t id = 0; id < aCount; ++id)
    {
        iThreads.emplace_back(GpThread());
    }

    //Main therad
    GpFlatMap<std::thread::id, GpTask*, 64>& tasksByThreadId = GpTaskAccessor::STasksByThreadId();

    GpTask* taskNullPtr = nullptr;

    tasksByThreadId.Insert
    (
        std::this_thread::get_id(),
        taskNullPtr
    );

    // Start threads
    iExecutorsCondVar = MakeSP<GpConditionVar>();

    for (GpThread& t: iThreads)
    {
        ExecutorT::SP executor = MakeSP<ExecutorT>(iExecutorsCondVar);
        iExecutors.emplace_back(executor);

        std::thread::id threadId = t.Run(executor);

        tasksByThreadId.Insert
        (
            threadId,
            taskNullPtr
        );
    }

    iExecutorsLeft = aCount;
}

GpTaskExecutorsPool::value_type GpTaskExecutorsPool::NewElement (GpSpinlock& /*aLocked*/)
{
    THROW_COND_GP
    (
        iExecutorsLeft > 0,
        "iExecutorsLeft > 0_cnt"_sv
    );

    const size_t id = NumOps::SSub(iExecutors.size(), iExecutorsLeft);

    return iExecutors.at(id).Pn();
}

void    GpTaskExecutorsPool::OnClear (void) noexcept
{
    for (GpThread& thread: iThreads)
    {
        thread.RequestStop();
    }

    for (GpThread& thread: iThreads)
    {
        thread.Join();
    }

    iThreads.clear();
    iExecutors.clear();
    iExecutorsLeft = 0;
    iExecutorsCondVar.Clear();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
