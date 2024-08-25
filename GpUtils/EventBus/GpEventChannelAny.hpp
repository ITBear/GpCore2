#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined (GP_USE_EVENT_BUS)

#include <GpCore2/GpUtils/EventBus/GpEventChannel.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>

namespace GPlatform {

using GpEventChannelAny = GpEventChannel<GpUUID, GpAny>;

}// namespace GPlatform

#endif// #if defined (GP_USE_EVENT_BUS)
