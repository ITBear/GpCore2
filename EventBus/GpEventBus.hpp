#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "GpEvent.hpp"
#include "GpEventSubscriber.hpp"

namespace GPlatform {

class GPCORE_API GpEventBus
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpEventBus);
	CLASS_DECLARE_DEFAULTS(GpEventBus);

public:
									GpEventBus	(void) noexcept;
									~GpEventBus	(void) noexcept;

	void							Clear		(void) noexcept;

	void							Subscribe	(GpEventSubscriber::SP aSubscriber);
	void							Unsubscribe	(GpEventSubscriber::SP aSubscriber);
	//void							Post		(GpEvent::SP aEvent);

private:
	GpSpinlock						iLock;
	GpEventSubscriber::C::Set::SP	iSubscribers;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)