#pragma once

#include <array>
#include <functional>
#include <chrono>
#include <thread>

namespace GPlatform {

class GpSleepStrategy
{
public:
    using ConditionFnT  = std::function<bool()>;

public:
    template<size_t N>
    static void         SWaitFor    (ConditionFnT                                                       aCondFn,
                                     const std::array<std::pair<size_t, std::chrono::milliseconds>, N>& aTryStages,
                                     std::chrono::milliseconds                                          aMaxSleepInterval);
};

template<size_t N>
void    GpSleepStrategy::SWaitFor
(
    ConditionFnT                                                        aCondFn,
    const std::array<std::pair<size_t, std::chrono::milliseconds>, N>&  aTryStages,
    const std::chrono::milliseconds                                     aMaxSleepInterval
)
{
    size_t trysCounter  = 0;
    size_t trysStageId  = 0;

    while (!aCondFn())
    {
        if (trysStageId < std::size(aTryStages)) [[likely]]
        {
            const auto[maxTrysCount, sleepInterval] = aTryStages.at(trysStageId);

            trysCounter++;
            if (trysCounter > maxTrysCount) [[unlikely]]
            {
                trysStageId++;
                trysCounter = 0;
                continue;
            }

            if (sleepInterval == std::chrono::milliseconds(0))
            {
                continue;
            }

            std::this_thread::sleep_for(sleepInterval);
        } else
        {
            std::this_thread::sleep_for(aMaxSleepInterval);
        }
    }
}

}// namespace GPlatform
