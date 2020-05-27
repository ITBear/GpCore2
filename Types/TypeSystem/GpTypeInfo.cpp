#include "GpTypeInfo.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpTypeInfo::GpTypeInfo (const GpTypeInfo& aTypeInfo):
iUID(aTypeInfo.iUID),
iInheritanceUID(aTypeInfo.iInheritanceUID),
iName(aTypeInfo.iName),
iType(aTypeInfo.iType),
iContainer(aTypeInfo.iContainer),
iContainerKeyType(aTypeInfo.iContainerKeyType),
iProps(aTypeInfo.iProps)
{
}

GpTypeInfo::GpTypeInfo (GpTypeInfo&& aTypeInfo) noexcept:
iUID(std::move(aTypeInfo.iUID)),
iInheritanceUID(std::move(aTypeInfo.iInheritanceUID)),
iName(std::move(aTypeInfo.iName)),
iType(std::move(aTypeInfo.iType)),
iContainer(std::move(aTypeInfo.iContainer)),
iContainerKeyType(std::move(aTypeInfo.iContainerKeyType)),
iProps(std::move(aTypeInfo.iProps))
{
}

GpTypeInfo::~GpTypeInfo (void) noexcept
{
}

GpTypeInfo&	GpTypeInfo::operator= (const GpTypeInfo& aTypeInfo)
{
	iUID				= aTypeInfo.iUID;
	iInheritanceUID		= aTypeInfo.iInheritanceUID;
	iName				= aTypeInfo.iName;
	iType				= aTypeInfo.iType;
	iContainer			= aTypeInfo.iContainer;
	iContainerKeyType	= aTypeInfo.iContainerKeyType;
	iProps				= aTypeInfo.iProps;

	return *this;
}

GpTypeInfo&	GpTypeInfo::operator= (GpTypeInfo&& aTypeInfo) noexcept
{
	iUID				= std::move(aTypeInfo.iUID);
	iInheritanceUID		= std::move(aTypeInfo.iInheritanceUID);
	iName				= std::move(aTypeInfo.iName);
	iType				= std::move(aTypeInfo.iType);
	iContainer			= std::move(aTypeInfo.iContainer);
	iContainerKeyType	= std::move(aTypeInfo.iContainerKeyType);
	iProps				= std::move(aTypeInfo.iProps);

	return *this;
}

}//GPlatform

#endif//GP_USE_REFLECTION