#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"

namespace GPlatform {

template<typename T>
class GpItcProducer;

template<typename T>
class GpItcConsumer;

template<typename T>
class GpItcProducerConsumer
{
    friend class GpItcProducer<T>;
    friend class GpItcConsumer<T>;

public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcProducerConsumer)
    CLASS_DD(GpItcProducerConsumer<T>)

    using ItcResultT = GpItcResult<T>;

public:
    inline                                          GpItcProducerConsumer   (const size_t aQueueMaxSize) noexcept;
                                                    ~GpItcProducerConsumer  (void) noexcept = default;

    [[nodiscard]] bool                              Produce                 (typename ItcResultT::SP    aResult,
                                                                             const milliseconds_t       aWaitTimeout);
    [[nodiscard]] typename ItcResultT::C::Opt::SP   Consume                 (const milliseconds_t       aWaitTimeout);

    void                                            Wakeup                  (void);

private:
    size_t                                          iQueueMaxSize = 0;
    typename ItcResultT::C::Deque::SP               iQueue;
    GpItcSharedCondition                            iReadySC;
};

template<typename T>
GpItcProducerConsumer<T>::GpItcProducerConsumer (const size_t aQueueMaxSize) noexcept:
iQueueMaxSize(aQueueMaxSize)
{
}

template<typename T>
bool    GpItcProducerConsumer<T>::Produce
(
    typename ItcResultT::SP aResult,
    const milliseconds_t    aWaitTimeout
)
{
    GpItcSharedCondition::WaitForConditionResT waitRes = iReadySC.WaitForCondition
    (
        aWaitTimeout,
        [&]()//Condition
        {
            return iQueue.size() < iQueueMaxSize;
        },
        [&]()//Action on condition met
        {
            iQueue.emplace_back(std::move(aResult));
        },
        [&]()//Action on condition not met
        {
            return GpItcSharedCondition::ActionCondNotMetRes::CONTINUE;
        }
    );

    if (waitRes == GpItcSharedCondition::WaitForConditionResT::OK)
    {
        iReadySC.WakeupAll([](){});
        return true;
    } else
    {
        return false;
    }
}

template<typename T>
typename GpItcProducerConsumer<T>::ItcResultT::C::Opt::SP   GpItcProducerConsumer<T>::Consume (const milliseconds_t aWaitTimeout)
{
    typename ItcResultT::C::Opt::SP res;

    GpItcSharedCondition::WaitForConditionResT waitRes = iReadySC.WaitForCondition
    (
        aWaitTimeout,
        [&]()//Condition
        {
            return iQueue.size() > 0;
        },
        [&]()//Action on condition met
        {
            typename ItcResultT::SP result = iQueue.front();
            iQueue.pop_front();

            res = std::move(result);
        },
        [&]()//Action on condition not met
        {
            return GpItcSharedCondition::ActionCondNotMetRes::CONTINUE;
        }
    );

    if (waitRes == GpItcSharedCondition::WaitForConditionResT::OK)
    {
        iReadySC.WakeupAll([](){});
    }

    return res;
}

template<typename T>
void    GpItcProducerConsumer<T>::Wakeup (void)
{
    iReadySC.WakeupAll([](){});
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
