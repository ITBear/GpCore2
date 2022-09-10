#pragma once

#include "GpItcFlag.hpp"

namespace GPlatform {

class GpItcProducerConsumer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcProducerConsumer)
    CLASS_DECLARE_DEFAULTS(GpItcProducerConsumer)

public:
                                                    GpItcProducerConsumer   (void) noexcept = default;
    inline                                          GpItcProducerConsumer   (const size_t aLimit) noexcept;
                                                    ~GpItcProducerConsumer  (void) noexcept = default;

    [[nodiscard]] inline bool                       Produce                 (GpItcResult::SP        aResult,
                                                                             const milliseconds_t   aWaitTimeout);
    [[nodiscard]] inline GpItcResult::C::Opt::SP    Consume                 (const milliseconds_t   aWaitTimeout);

private:
    mutable GpSpinlock                              iLock;
    size_t                                          iLimit = 0;
    GpItcResult::C::Deque::SP                       iQueue;
    GpItcFlag                                       iReadyToConsumeFlag;
    GpItcFlag                                       iReadyToProduceFlag;
};

GpItcProducerConsumer::GpItcProducerConsumer (const size_t aLimit) noexcept:
iLimit(aLimit)
{
}

bool    GpItcProducerConsumer::Produce
(
    GpItcResult::SP         aResult,
    const milliseconds_t    aWaitTimeout
)
{
    std::scoped_lock lock(iLock);

    while (iQueue.size() >= iLimit)
    {
        if (aWaitTimeout > 0.0_si_ms)
        {
            iReadyToProduceFlag.Reset();
            if (iReadyToProduceFlag.TryWait(iLock, aWaitTimeout) == false)
            {
                return false;
            }
        } else
        {
            return false;
        }
    }

    //Produce
    iQueue.emplace_back(std::move(aResult));
    iReadyToConsumeFlag.TestAndSet();

    return true;
}

GpItcResult::C::Opt::SP GpItcProducerConsumer::Consume (const milliseconds_t aWaitTimeout)
{
    std::scoped_lock lock(iLock);

    while (iQueue.size() == 0)
    {
        if (aWaitTimeout > 0.0_si_ms)
        {
            iReadyToConsumeFlag.Reset();
            if (iReadyToConsumeFlag.TryWait(iLock, aWaitTimeout) == false)
            {
                return std::nullopt;
            }
        } else
        {
            return std::nullopt;
        }
    }

    //Consume
    GpItcResult::SP result = iQueue.front();
    iQueue.pop_front();
    iReadyToProduceFlag.TestAndSet();

    return result;
}

}//namespace GPlatform
