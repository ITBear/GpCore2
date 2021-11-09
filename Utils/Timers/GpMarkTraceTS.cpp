#include "GpMarkTraceTS.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

GpMarkTraceTS::GpMarkTraceTS (void) noexcept
{
}

GpMarkTraceTS::GpMarkTraceTS (const GpMarkTraceTS& aTrace):
iMarks(aTrace.iMarks)
{
}

GpMarkTraceTS::GpMarkTraceTS (GpMarkTraceTS&& aTrace) noexcept:
iMarks(std::move(aTrace.iMarks))
{
}

GpMarkTraceTS::~GpMarkTraceTS (void) noexcept
{
}

void    GpMarkTraceTS::Mark
(
    std::string_view        aComment,
    const SourceLocationT&  aSourceLocation
)
{
    GpMarkTS m(aComment, aSourceLocation);
    iMarks.emplace_back(std::move(m));
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
