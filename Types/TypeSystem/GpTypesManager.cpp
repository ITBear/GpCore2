#include "GpTypesManager.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpTypesManager::GpTypesManager (void) noexcept
{
}

GpTypesManager::~GpTypesManager (void) noexcept
{
	iElements.Clear();
}

GpTypesManager&	GpTypesManager::S (void) noexcept
{
	static GpTypesManager m;
	return m;
}

void	GpTypesManager::Register (const GpTypeInfo& aTypeInfo)
{
	iElements.Register(aTypeInfo.UID(), aTypeInfo);
}

void	GpTypesManager::Register (GpTypeInfo&& aTypeInfo)
{
	const GpUUID uid = aTypeInfo.UID();
	iElements.Register(uid, std::move(aTypeInfo));
}

void	GpTypesManager::Unregister (const GpUUID& aTypeUID)
{
	iElements.Unregister(aTypeUID);
}

const GpTypeInfo::C::Opt::CRef	GpTypesManager::Find (const GpUUID& aTypeUID) const noexcept
{
	return iElements.Find(aTypeUID);
}

}//GPlatform

#endif//GP_USE_REFLECTION