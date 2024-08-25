#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

struct GpDataProcessUpdateEvent
{
    double          iProgress       = 0.0;  // [0.0...100.0]
    u_int_64        iProcessedSize  = 0;
    u_int_64        iTotalSize      = 0;
};

class GpDataProcessUpdateEventEmitter
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDataProcessUpdateEventEmitter)

public:
    inline          GpDataProcessUpdateEventEmitter     (u_int_64   aTotalSize,
                                                         double     aEmitDelta  = 0.1) noexcept;
                    ~GpDataProcessUpdateEventEmitter    (void) noexcept = default;

    template<typename GpEventChannelT>
    void            Update                              (u_int_64           aProcessedSize,
                                                         GpEventChannelT&   aEventChannel);

private:
    const u_int_64  iTotalSize;
    const double    iEmitDelta;
    double          iValue  = 0.0;
};

GpDataProcessUpdateEventEmitter::GpDataProcessUpdateEventEmitter
(
    const u_int_64  aTotalSize,
    const double    aEmitDelta
) noexcept:
iTotalSize{aTotalSize},
iEmitDelta{aEmitDelta}
{
}

template<typename GpEventChannelT>
void    GpDataProcessUpdateEventEmitter::Update
(
    const u_int_64      aProcessedSize,
    GpEventChannelT&    aEventChannel
)
{
    const double currentValue   = (double(aProcessedSize) / double(iTotalSize)) * 100.0;
    const double delta          = currentValue - iValue;

    if (   (delta >= iEmitDelta)
        || (currentValue >= 100.0))[[unlikely]]
    {
        iValue = currentValue;
        aEventChannel.PushEvent
        (
            GpDataProcessUpdateEvent
            {
                .iProgress      = currentValue,
                .iProcessedSize = aProcessedSize,
                .iTotalSize     = iTotalSize
            }
        );
    }
}

}// namespace GPlatform

#endif// #if defined (GP_USE_EVENT_BUS)
