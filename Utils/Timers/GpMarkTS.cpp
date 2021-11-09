#include "GpMarkTS.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

GpMarkTS::GpMarkTS (void) noexcept
{
}

GpMarkTS::GpMarkTS (const GpMarkTS& aTag):
iHiResTS(aTag.iHiResTS),
iSteadyTS(aTag.iSteadyTS),
iComment(aTag.iComment),
iSourceLocation(aTag.iSourceLocation)
{
}

GpMarkTS::GpMarkTS (GpMarkTS&& aTag) noexcept:
iHiResTS(std::move(aTag.iHiResTS)),
iSteadyTS(std::move(aTag.iSteadyTS)),
iComment(std::move(aTag.iComment)),
iSourceLocation(std::move(aTag.iSourceLocation))
{
}

GpMarkTS::GpMarkTS (std::string_view aComment):
iHiResTS(GpDateTimeOps::SHighResTS_us()),
iSteadyTS(GpDateTimeOps::SSteadyTS_us() - GpDateTimeOps::SSteadyTS_us_AtAppStart()),
iComment(aComment)
{
}

GpMarkTS::GpMarkTS (const SourceLocationT& aSourceLocation):
iHiResTS(GpDateTimeOps::SHighResTS_us()),
iSteadyTS(GpDateTimeOps::SSteadyTS_us() - GpDateTimeOps::SSteadyTS_us_AtAppStart()),
iSourceLocation(aSourceLocation)
{
}

GpMarkTS::GpMarkTS
(
    std::string_view        aComment,
    const SourceLocationT&  aSourceLocation
):
iHiResTS(GpDateTimeOps::SHighResTS_us()),
iSteadyTS(GpDateTimeOps::SSteadyTS_us() - GpDateTimeOps::SSteadyTS_us_AtAppStart()),
iComment(aComment),
iSourceLocation(aSourceLocation)
{
}

GpMarkTS::~GpMarkTS (void) noexcept
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
