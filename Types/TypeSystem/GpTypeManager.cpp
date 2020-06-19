#include "GpTypeManager.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpTypeManager::GpTypeManager (void) noexcept
{
}

GpTypeManager::~GpTypeManager (void) noexcept
{
	iElements.Clear();
}

GpTypeManager&	GpTypeManager::S (void) noexcept
{
	static GpTypeManager m;
	return m;
}

void	GpTypeManager::Register (const GpTypeInfo& aTypeInfo)
{
	iElements.Register(aTypeInfo.UID(), aTypeInfo);
}

void	GpTypeManager::Register (GpTypeInfo&& aTypeInfo)
{
	const GpUUID& uid = aTypeInfo.UID();
	iElements.Register(uid, std::move(aTypeInfo));
}

void	GpTypeManager::Unregister (const GpUUID& aTypeUID)
{
	iElements.Unregister(aTypeUID);
}

GpTypeInfo::C::Opt::CRef	GpTypeManager::Find (const GpUUID& aTypeUID) const noexcept
{
	return iElements.Find(aTypeUID);
}

bool	GpTypeManager::IsBaseOf (const GpUUID& aBaseTypeUID, const GpUUID& aDerivedTypeUID) const noexcept
{
	if (   aBaseTypeUID.IsZero()
		|| aDerivedTypeUID.IsZero())
	{
		return false;
	}

	if (aDerivedTypeUID == aBaseTypeUID)
	{
		return true;
	}

	GpTypeInfo::C::Opt::CRef typeInfoOpt = Find(aDerivedTypeUID);

	while (typeInfoOpt.has_value())
	{
		const GpTypeInfo& typeInfo = typeInfoOpt.value();

		if (typeInfo.UID() == aBaseTypeUID)
		{
			return true;
		}

		typeInfoOpt = Find(typeInfo.BaseUID());
	}

	return false;
}

}//GPlatform

#endif//GP_USE_REFLECTION
