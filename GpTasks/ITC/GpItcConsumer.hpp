#pragma once

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

class GpItcConsumer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcConsumer)
    CLASS_DD(GpItcConsumer)

public:
    inline                                          GpItcConsumer   (GpItcProducerConsumer::SP aProducerConsumer) noexcept;
                                                    ~GpItcConsumer  (void) noexcept = default;

    [[nodiscard]] inline GpItcResult::C::Opt::SP    Consume         (const milliseconds_t aWaitTimeout);

private:
    GpItcProducerConsumer::SP   iProducerConsumer;
};

GpItcConsumer::GpItcConsumer (GpItcProducerConsumer::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

GpItcResult::C::Opt::SP GpItcConsumer::Consume (const milliseconds_t aWaitTimeout)
{
    return iProducerConsumer.Vn().Consume(aWaitTimeout);
}

}//namespace GPlatform
