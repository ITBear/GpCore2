#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcProducerConsumer.hpp"

namespace GPlatform {

template<typename T>
class GpItcConsumer
{
public:
    CLASS_DD(GpItcConsumer<T>)

    using ItcProducerConsumerT  = GpItcProducerConsumer<T>;
    using ItcResultT            = GpItcResult<T>;

public:
                                            GpItcConsumer   (void) noexcept = delete;
    inline                                  GpItcConsumer   (typename  ItcProducerConsumerT::SP aProducerConsumer) noexcept;
    inline                                  GpItcConsumer   (const GpItcConsumer& aConsumer) noexcept;
    inline                                  GpItcConsumer   (GpItcConsumer&& aConsumer) noexcept;
    inline                                  ~GpItcConsumer  (void) noexcept;

    inline typename ItcResultT::C::Opt::Val Consume         (const milliseconds_t aWaitTimeout);

private:
    typename  ItcProducerConsumerT::SP      iProducerConsumer;
};

template<typename T>
GpItcConsumer<T>::GpItcConsumer (typename  ItcProducerConsumerT::SP aProducerConsumer) noexcept:
iProducerConsumer(std::move(aProducerConsumer))
{
}

template<typename T>
GpItcConsumer<T>::GpItcConsumer (const GpItcConsumer& aConsumer) noexcept:
iProducerConsumer(aConsumer.iProducerConsumer)
{
}

template<typename T>
GpItcConsumer<T>::GpItcConsumer (GpItcConsumer&& aConsumer) noexcept:
iProducerConsumer(std::move(aConsumer.iProducerConsumer))
{
}

template<typename T>
GpItcConsumer<T>::~GpItcConsumer (void) noexcept
{
}

template<typename T>
typename GpItcConsumer<T>::ItcResultT::C::Opt::Val  GpItcConsumer<T>::Consume (const milliseconds_t aWaitTimeout)
{
    ItcProducerConsumerT* pc = iProducerConsumer.Pn();

    if (pc)
    {
        return pc->Consume(aWaitTimeout);
    } else
    {
        return std::nullopt;
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
