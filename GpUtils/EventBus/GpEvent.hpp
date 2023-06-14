#pragma once

#include "../../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "../Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GpUUID;

class GpEvent
{
public:
    CLASS_DD(GpEvent)

protected:
    explicit                GpEvent     (void) noexcept = default;
    explicit                GpEvent     (const GpEvent& aEvent) noexcept = default;
    explicit                GpEvent     (GpEvent&& aEvent) noexcept = default;

public:
    virtual                 ~GpEvent    (void) noexcept = default;

    virtual const GpUUID&   Type        (void) const noexcept = 0;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
