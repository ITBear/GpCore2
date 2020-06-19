#include "GpTypeInfo.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpTypeInfo::GpTypeInfo (const GpTypeInfo& aTypeInfo):
iUID(aTypeInfo.iUID),
iBaseUID(aTypeInfo.iBaseUID),
iName(aTypeInfo.iName),
iType(aTypeInfo.iType),
iProps(aTypeInfo.iProps)
{
}

GpTypeInfo::GpTypeInfo (GpTypeInfo&& aTypeInfo) noexcept:
iUID(std::move(aTypeInfo.iUID)),
iBaseUID(std::move(aTypeInfo.iBaseUID)),
iName(std::move(aTypeInfo.iName)),
iType(std::move(aTypeInfo.iType)),
iProps(std::move(aTypeInfo.iProps))
{
}

GpTypeInfo::~GpTypeInfo (void) noexcept
{
}

GpTypeInfo&	GpTypeInfo::operator= (const GpTypeInfo& aTypeInfo)
{
	iUID				= aTypeInfo.iUID;
	iBaseUID			= aTypeInfo.iBaseUID;
	iName				= aTypeInfo.iName;
	iType				= aTypeInfo.iType;
	iProps				= aTypeInfo.iProps;

	return *this;
}

GpTypeInfo&	GpTypeInfo::operator= (GpTypeInfo&& aTypeInfo) noexcept
{
	iUID				= std::move(aTypeInfo.iUID);
	iBaseUID			= std::move(aTypeInfo.iBaseUID);
	iName				= std::move(aTypeInfo.iName);
	iType				= std::move(aTypeInfo.iType);
	iProps				= std::move(aTypeInfo.iProps);

	return *this;
}

}//GPlatform

#endif//GP_USE_REFLECTION
