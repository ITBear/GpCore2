#pragma once

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"

namespace GPlatform {

class GpItcProducer;
class GpItcConsumer;

class GP_TASKS_API GpItcProducerConsumer
{
    friend class GpItcProducer;
    friend class GpItcConsumer;

public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducerConsumer)
    CLASS_DD(GpItcProducerConsumer)

public:
    inline                                  GpItcProducerConsumer   (const size_t aQueueMaxSize) noexcept;
                                            ~GpItcProducerConsumer  (void) noexcept = default;

    [[nodiscard]] bool                      Produce                 (GpItcResult::SP        aResult,
                                                                     const milliseconds_t   aWaitTimeout);
    [[nodiscard]] GpItcResult::C::Opt::SP   Consume                 (const milliseconds_t   aWaitTimeout);

    void                                    Wakeup                  (void);

private:
    size_t                                  iQueueMaxSize = 0;
    GpItcResult::C::Deque::SP               iQueue;
    GpItcSharedCondition                    iReadySC;
};

GpItcProducerConsumer::GpItcProducerConsumer (const size_t aQueueMaxSize) noexcept:
iQueueMaxSize(aQueueMaxSize)
{
}

}//namespace GPlatform
