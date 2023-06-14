#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

template<typename T>
class GpItcConsumer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcConsumer)
    CLASS_DD(GpItcConsumer<T>)

    using ItcProducerConsumerT  = GpItcProducerConsumer<T>;
    using ItcResultT            = GpItcResult<T>;

public:
    inline                                                  GpItcConsumer   (typename  ItcProducerConsumerT::SP aProducerConsumer) noexcept;
    inline                                                  ~GpItcConsumer  (void) noexcept;

    [[nodiscard]] inline typename ItcResultT::C::Opt::SP    Consume         (const milliseconds_t aWaitTimeout);

private:
    typename  ItcProducerConsumerT::SP  iProducerConsumer;
};

template<typename T>
GpItcConsumer<T>::GpItcConsumer (typename  ItcProducerConsumerT::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

template<typename T>
GpItcConsumer<T>::~GpItcConsumer (void) noexcept
{
}

template<typename T>
typename GpItcConsumer<T>::ItcResultT::C::Opt::SP   GpItcConsumer<T>::Consume (const milliseconds_t aWaitTimeout)
{
    return iProducerConsumer.V().Consume(aWaitTimeout);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
