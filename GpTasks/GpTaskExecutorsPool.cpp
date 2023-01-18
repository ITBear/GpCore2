#include "GpTaskExecutorsPool.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskAccessor.hpp"

namespace GPlatform {

void    GpTaskExecutorsPool::Start (const size_t aCount)
{
    std::scoped_lock lock(iLock);

    iExecutors.fill(ExecutorT::SP::SNull());

    iCount = aCount;
    iThreads.resize(aCount);
    iExecutorsIdleFlags.set();

    //Main therad
    GpTaskAccessor::SAddExecutorThreadId(std::this_thread::get_id());

    //Start threads 
    for (size_t id = 0; id < aCount; id++)
    {
        GpThread&       thread      = iThreads.at(id);
        ExecutorT::SP   executor    = MakeSP<ExecutorT>(id, *this);

        iExecutors.at(id) = executor;

        const std::thread::id threadId = thread.Run(executor);
        GpTaskAccessor::SAddExecutorThreadId(threadId);
    }
}

void    GpTaskExecutorsPool::RequestStop (void) noexcept
{
    for (GpThread& thread: iThreads)
    {
        thread.RequestStop();
    }
}

void    GpTaskExecutorsPool::Join (void) noexcept
{
    for (GpThread& thread: iThreads)
    {
        thread.Join();
    }

    iThreads.clear();

    for (ExecutorT::SP& executor: iExecutors)
    {
        executor.Clear();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
