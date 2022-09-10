#pragma once

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

class GpItcProducer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducer)
    CLASS_DECLARE_DEFAULTS(GpItcProducer)

public:
    inline                      GpItcProducer   (GpItcProducerConsumer::SP aProducerConsumer) noexcept;
                                ~GpItcProducer  (void) noexcept = default;

    [[nodiscard]] inline bool   Produce         (GpItcResult::SP        aResult,
                                                 const milliseconds_t   aWaitTimeout);

private:
    GpItcProducerConsumer::SP   iProducerConsumer;
};

GpItcProducer::GpItcProducer (GpItcProducerConsumer::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

bool    GpItcProducer::Produce
(
    GpItcResult::SP         aResult,
    const milliseconds_t    aWaitTimeout
)
{
    return iProducerConsumer.Vn().Produce
    (
        std::move(aResult),
        aWaitTimeout
    );
}

}//namespace GPlatform
