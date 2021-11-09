#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "GpMarkTS.hpp"

namespace GPlatform {

class GPCORE_API GpMarkTraceTS
{
public:
    CLASS_DECLARE_DEFAULTS(GpMarkTraceTS)

public:
                                    GpMarkTraceTS   (void) noexcept;
                                    GpMarkTraceTS   (const GpMarkTraceTS& aTrace);
                                    GpMarkTraceTS   (GpMarkTraceTS&& aTrace) noexcept;
                                    ~GpMarkTraceTS  (void) noexcept;

    void                            Mark            (std::string_view       aComment,
                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current());

    const GpMarkTS::C::Vec::Val&    Marks           (void) const noexcept {return iMarks;}

private:
    GpMarkTS::C::Vec::Val           iMarks;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
