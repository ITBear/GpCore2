#include "GpItcProducerConsumer.hpp"

#include <iostream>

namespace GPlatform {

bool    GpItcProducerConsumer::Produce
(
    GpItcResult::SP         aResult,
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

GpItcResult::C::Opt::SP GpItcProducerConsumer::Consume (const milliseconds_t aWaitTimeout)
{
    GpItcResult::C::Opt::SP res;

    GpItcSharedCondition::WaitForConditionResT waitRes = iReadySC.WaitForCondition
    (
        aWaitTimeout,
        [&]()//Condition
        {
            return iQueue.size() > 0;
        },
        [&]()//Action on condition met
        {
            GpItcResult::SP result = iQueue.front();
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

void    GpItcProducerConsumer::Wakeup (void)
{
    iReadySC.WakeupAll([](){});
}

}//namespace GPlatform
