#include "GpItcFuture.hpp"
#include "GpTask.hpp"
#include "GpTaskScheduler.hpp"
#include "Fibers/GpTaskFiber.hpp"

#include <iostream>

namespace GPlatform {

bool    GpItcFuture::SetResult (GpItcResult::SP aResult) noexcept
{
    return iItcFlag.TestAndSet
    (
        std::move(aResult),
        [&](GpItcResult::SP&& _result){iResult = std::move(_result);}
    );
}

}//namespace GPlatform
