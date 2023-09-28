#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

template<typename T>
class GpItcProducer
{
public:
    CLASS_DD(GpItcProducer<T>)

    using ItcProducerConsumerT  = GpItcProducerConsumer<T>;
    using ItcResultT            = GpItcResult<T>;

public:
                                        GpItcProducer       (void) noexcept = delete;
    inline                              GpItcProducer       (typename ItcProducerConsumerT::SP aProducerConsumer) noexcept;
    inline                              GpItcProducer       (const GpItcProducer& aProducer) noexcept;
    inline                              GpItcProducer       (GpItcProducer&& aProducer) noexcept;
    inline                              ~GpItcProducer      (void) noexcept;

    inline bool                         Produce             (ItcResultT&&           aResult,
                                                             const milliseconds_t   aWaitTimeout);

    typename ItcProducerConsumerT::SP   ProducerConsumer    (void) noexcept {return iProducerConsumer;}

private:
    typename ItcProducerConsumerT::SP   iProducerConsumer;
};

template<typename T>
GpItcProducer<T>::GpItcProducer (typename ItcProducerConsumerT::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

template<typename T>
GpItcProducer<T>::GpItcProducer (const GpItcProducer& aProducer) noexcept:
iProducerConsumer(aProducer.iProducerConsumer)
{

}

template<typename T>
GpItcProducer<T>::GpItcProducer (GpItcProducer&& aProducer) noexcept:
iProducerConsumer(std::move(aProducer.iProducerConsumer))
{
}

template<typename T>
GpItcProducer<T>::~GpItcProducer (void) noexcept
{
}

template<typename T>
bool    GpItcProducer<T>::Produce
(
    ItcResultT&&            aResult,
    const milliseconds_t    aWaitTimeout
)
{
    if (iProducerConsumer.IsNotNULL()) [[likely]]
    {
        return iProducerConsumer.Vn().Produce(std::move(aResult), aWaitTimeout);
    } else
    {
        return false;
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
