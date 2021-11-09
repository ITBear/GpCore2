#pragma once

#include "../../GpCore_global.hpp"
#include "../../Config/GpConfig.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"
#include "../../Types/Containers/GpElementsCatalog.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

class GPCORE_API GpMarkTS
{
public:
    CLASS_DECLARE_DEFAULTS(GpMarkTS)

public:
    using SourceLocationOptT = std::optional<SourceLocationT>;

public:
                                GpMarkTS        (void) noexcept;
                                GpMarkTS        (const GpMarkTS& aTag);
                                GpMarkTS        (GpMarkTS&& aTag) noexcept;
    explicit                    GpMarkTS        (std::string_view       aComment);
    explicit                    GpMarkTS        (const SourceLocationT& aSourceLocation);
    explicit                    GpMarkTS        (std::string_view       aComment,
                                                 const SourceLocationT& aSourceLocation);
                                ~GpMarkTS       (void) noexcept;

    microseconds_t              HiResTS         (void) const noexcept {return iHiResTS;}
    microseconds_t              SteadyTS        (void) const noexcept {return iSteadyTS;}
    std::string_view            Comment         (void) const noexcept {return iComment;}
    const SourceLocationOptT&   SourceLocation  (void) const noexcept {return iSourceLocation;}

private:
    microseconds_t              iHiResTS;
    microseconds_t              iSteadyTS;
    std::string                 iComment;
    SourceLocationOptT          iSourceLocation;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
