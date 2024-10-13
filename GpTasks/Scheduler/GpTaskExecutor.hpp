#pragma once

#include <GpCore2/GpUtils/Threads/GpRunnable.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskExecutor: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskExecutor)
    CLASS_DD(GpTaskExecutor)

protected:
                        GpTaskExecutor  (void) noexcept = default;

public:
    virtual             ~GpTaskExecutor (void) noexcept = default;

    virtual void        Run             (std::atomic_flag& aStopRequest) noexcept = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
