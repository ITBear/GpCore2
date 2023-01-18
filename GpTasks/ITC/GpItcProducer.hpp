#pragma once

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

class GpItcProducer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducer)
    CLASS_DD(GpItcProducer)

public:
    inline                      GpItcProducer   (GpItcProducerConsumer::SP aProducerConsumer) noexcept;
    inline                      ~GpItcProducer  (void) noexcept;

    [[nodiscard]] inline bool   Produce         (GpItcResult::SP        aResult,
                                                 const milliseconds_t   aWaitTimeout);

private:
    GpItcProducerConsumer::SP   iProducerConsumer;
};

GpItcProducer::GpItcProducer (GpItcProducerConsumer::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

GpItcProducer::~GpItcProducer (void) noexcept
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
