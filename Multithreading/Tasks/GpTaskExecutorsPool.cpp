#include "GpTaskExecutorsPool.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskExecutorsPool::GpTaskExecutorsPool (void) noexcept
{
}

GpTaskExecutorsPool::~GpTaskExecutorsPool (void) noexcept
{   
}

void    GpTaskExecutorsPool::SetScheduler (GpWP<GpTaskScheduler> aScheduler) noexcept
{
    iScheduler = std::move(aScheduler);
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

void    GpTaskExecutorsPool::WakeupAll (void) noexcept
{
    iExecutorsCondVar.Vn().WakeupAll();
}

void    GpTaskExecutorsPool::WakeupOne (void) noexcept
{
    iExecutorsCondVar.Vn().WakeupOne();
}

void    GpTaskExecutorsPool::PreInit (const count_t aCount)
{
    iExecutors.reserve(aCount.As<size_t>());
    iThreads.reserve(aCount.As<size_t>());

    // Create threads
    for (count_t id = 0_cnt; id < aCount; ++id)
    {
        GpThread t;     
        iThreads.emplace_back(std::move(t));
    }

    // Start threads
    iExecutorsCondVar = MakeSP<GpConditionVar>();
    for (GpThread& t: iThreads)
    {
        ExecutorT::SP executor = MakeSP<ExecutorT>(iScheduler, iExecutorsCondVar);
        iExecutors.emplace_back(executor);

        t.Run(executor);
    }

    iExecutorsLeft = aCount;
}

GpTaskExecutorsPool::value_type GpTaskExecutorsPool::NewElement (void)
{
    THROW_GPE_COND(iExecutorsLeft > 0_cnt);

    const count_t id = count_t::SMake(iExecutors.size()) - iExecutorsLeft;

    return iExecutors.at(id.As<size_t>()).Pn();
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
    iExecutorsLeft = 0_cnt;
    iExecutorsCondVar.Clear();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
