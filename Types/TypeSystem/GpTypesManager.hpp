#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpTypeInfo.hpp"
#include "../../Multithreading/SyncPrimitives/GpRWLock.hpp"
#include "../Containers/GpElementsCatalog.hpp"

namespace GPlatform {

class GPCORE_API GpTypesManager
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTypesManager);
	CLASS_DECLARE_DEFAULTS(GpTypesManager);
	CLASS_TAG(THREAD_SAFE)

	using ElementsT	= GpElementsCatalog<GpUUID, GpTypeInfo, GpMap>;

public:
									GpTypesManager	(void) noexcept;
									~GpTypesManager	(void) noexcept;

	static GpTypesManager&			S				(void) noexcept;

	void							Register		(const GpTypeInfo& aTypeInfo);
	void							Register		(GpTypeInfo&& aTypeInfo);
	void							Unregister		(const GpUUID& aTypeUID);
	const GpTypeInfo::C::Opt::CRef	Find			(const GpUUID& aTypeUID) const noexcept;

private:
	ElementsT						iElements;
};

}//GPlatform

#endif//GP_USE_REFLECTION