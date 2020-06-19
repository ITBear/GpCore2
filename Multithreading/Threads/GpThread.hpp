#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpRunnable.hpp"

namespace GPlatform {

class GPCORE_API GpThread
{
public:
#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
    using ImplT = std::jthread;
#endif//#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)

    CLASS_DECLARE_DEFAULTS(GpThread);

public:
                        GpThread    (void) noexcept;
                        GpThread    (std::string_view aName);
                        GpThread    (const GpThread& aThread) = delete;
                        GpThread    (GpThread&& aThread) noexcept;
                        ~GpThread   (void) noexcept;

    std::string_view    Name        (void) const noexcept {return iName;}
    void                Run         (GpRunnable::SP aRunnable);

    bool                Joinable    (void) const noexcept;
    void                Join        (void) noexcept;
    bool                RequestStop (void) noexcept;

private:
    mutable std::mutex  iMutex;
    std::string         iName;
    ImplT               iThread;
    GpRunnable::SP      iRunnable;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
