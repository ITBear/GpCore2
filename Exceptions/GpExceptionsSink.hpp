#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpException.hpp"
#include <optional>
#include <functional>
#include <shared_mutex>

namespace GPlatform{

class GPCORE_API GpExceptionsSink
{
public:
    using RefT = std::reference_wrapper<GpExceptionsSink>;

protected:
                                GpExceptionsSink    (void) noexcept = default;
                                GpExceptionsSink    (const GpExceptionsSink&) = delete;
                                GpExceptionsSink    (GpExceptionsSink&&) = delete;
                                ~GpExceptionsSink   (void) noexcept = default;

    virtual void                OnSink              (std::string_view       aMsg,
                                                     const SourceLocationT& aLocation) noexcept;

public:
    static void                 SSetGlobalSink      (GpExceptionsSink& sSink) noexcept;

    static void                 SSink               (const std::exception&  aException,
                                                     const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;
    static void                 SSink               (std::string_view aMsg,
                                                     const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;
    static void                 SSinkUnknown        (const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;

private:
    static std::shared_mutex    sExceptionsSinkMutex;
    static GpExceptionsSink     sDefaultExceptionsSink;
    static RefT                 sExceptionsSink;
};

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
