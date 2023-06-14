#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

template<typename T>
class GpItcProducer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducer)
    CLASS_DD(GpItcProducer<T>)

    using ItcProducerConsumerT  = GpItcProducerConsumer<T>;
    using ItcResultT            = GpItcResult<T>;

public:
    inline                              GpItcProducer   (typename ItcProducerConsumerT::SP aProducerConsumer) noexcept;
    inline                              ~GpItcProducer  (void) noexcept;

    [[nodiscard]] inline bool           Produce         (typename ItcResultT::SP    aResult,
                                                         const milliseconds_t       aWaitTimeout);

private:
    typename ItcProducerConsumerT::SP   iProducerConsumer;
};

template<typename T>
GpItcProducer<T>::GpItcProducer (typename ItcProducerConsumerT::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

template<typename T>
GpItcProducer<T>::~GpItcProducer (void) noexcept
{
}

template<typename T>
bool    GpItcProducer<T>::Produce
(
    typename ItcResultT::SP aResult,
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

#endif//#if defined(GP_USE_MULTITHREADING)
