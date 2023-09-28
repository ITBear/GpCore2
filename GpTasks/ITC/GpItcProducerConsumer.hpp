#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <shared_mutex>

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"
#include "../../GpUtils/SyncPrimitives/GpRWSpinLock.hpp"
#include "../../GpUtils/Types/Strings/GpStringOps.hpp"
#include "../../GpUtils/Types/Strings/GpStringUtils.hpp"
#include "../../GpUtils/SyncPrimitives/GpUnlockGuard.hpp"

namespace GPlatform {

template<typename T>
class GpItcProducerConsumer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducerConsumer)
    CLASS_DD(GpItcProducerConsumer<T>)

    using value_type    = T;
    using ItcResultT    = GpItcResult<T>;

public:
    inline                              GpItcProducerConsumer   (const size_t aQueueMaxSize) noexcept;
    inline                              ~GpItcProducerConsumer  (void) noexcept;

    bool                                Produce                 (ItcResultT&&           aResult,
                                                                 const milliseconds_t   aWaitTimeout);
    typename ItcResultT::C::Opt::Val    Consume                 (const milliseconds_t   aWaitTimeout);
    typename ItcResultT::C::Deque::Val  ExtractQueue            (void) noexcept;

    void                                NotifyAll               (void);

private:
    const size_t                        iQueueMaxSize = 0;
    typename ItcResultT::C::Deque::Val  iQueue;
    mutable GpRWSpinLock                iQueueLock;

    GpItcSharedCondition                iProducersSC;
    GpItcSharedCondition                iConsumersSC;
};

template<typename T>
GpItcProducerConsumer<T>::GpItcProducerConsumer (const size_t aQueueMaxSize) noexcept:
iQueueMaxSize(aQueueMaxSize)
{
}

template<typename T>
GpItcProducerConsumer<T>::~GpItcProducerConsumer (void) noexcept
{
    try
    {
        iProducersSC.Notify(GpItcSharedCondition::NotifyMode::ALL);
        iConsumersSC.Notify(GpItcSharedCondition::NotifyMode::ALL);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpItcProducerConsumer::~GpItcProducerConsumer]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpItcProducerConsumer::~GpItcProducerConsumer]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpItcProducerConsumer::~GpItcProducerConsumer]: unknown exception"_sv);
    }
}

template<typename T>
bool    GpItcProducerConsumer<T>::Produce
(
    ItcResultT&&            aResult,
    const milliseconds_t    aWaitTimeout
)
{
    return iProducersSC.WaitFor
    (
        aWaitTimeout,
        [&](std::mutex& aLock)//Condition
        {
            std::shared_lock lock(iQueueLock);
            if (iQueue.size() >= iQueueMaxSize)
            {
                return false;
            }

            iQueue.emplace_back(std::move(aResult));

            {
                GpUnlockGuard unlock(aLock);
                iConsumersSC.Notify(GpItcSharedCondition::NotifyMode::ONE);
            }

            return true;
        }
    );
}

template<typename T>
typename GpItcProducerConsumer<T>::ItcResultT::C::Opt::Val  GpItcProducerConsumer<T>::Consume (const milliseconds_t aWaitTimeout)
{
    typename ItcResultT::C::Opt::Val res;

    iConsumersSC.WaitFor
    (
        aWaitTimeout,
        [&](std::mutex& aLock)//Condition
        {
            std::shared_lock lock(iQueueLock);

            if (iQueue.empty())
            {
                return false;
            }

            res = std::move(iQueue.front());
            iQueue.pop_front();

            {
                GpUnlockGuard unlock(aLock);
                iProducersSC.Notify(GpItcSharedCondition::NotifyMode::ONE);
            }

            return true;
        }
    );

    return res;
}

template<typename T>
typename GpItcProducerConsumer<T>::ItcResultT::C::Deque::Val    GpItcProducerConsumer<T>::ExtractQueue (void) noexcept
{
    std::scoped_lock lock(iQueueLock);
    return std::move(iQueue);
}

template<typename T>
void    GpItcProducerConsumer<T>::NotifyAll (void)
{
    iProducersSC.Notify(GpItcSharedCondition::NotifyMode::ALL);
    iConsumersSC.Notify(GpItcSharedCondition::NotifyMode::ALL);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
