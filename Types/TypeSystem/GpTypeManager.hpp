#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpTypeInfo.hpp"
#include "../../Multithreading/SyncPrimitives/GpRWLock.hpp"
#include "../Containers/GpElementsCatalog.hpp"

namespace GPlatform {

class GPCORE_API GpTypeManager
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTypeManager);
	CLASS_DECLARE_DEFAULTS(GpTypeManager);
	CLASS_TAG(THREAD_SAFE)

	using ElementsT	= GpElementsCatalog<GpUUID, GpTypeInfo, GpMap>;

public:
								GpTypeManager	(void) noexcept;
								~GpTypeManager	(void) noexcept;

	static GpTypeManager&		S				(void) noexcept;

	void						Register		(const GpTypeInfo& aTypeInfo);
	void						Register		(GpTypeInfo&& aTypeInfo);
	void						Unregister		(const GpUUID& aTypeUID);
	GpTypeInfo::C::Opt::CRef	Find			(const GpUUID& aTypeUID) const noexcept;
	bool						IsBaseOf		(const GpUUID& aBaseTypeUID, const GpUUID& aDerivedTypeUID) const noexcept;

private:
	ElementsT					iElements;
};

}//GPlatform

#endif//GP_USE_REFLECTION
