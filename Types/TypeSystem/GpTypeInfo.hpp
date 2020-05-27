#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpType.hpp"
#include "GpTypeContainer.hpp"
#include "GpTypePropInfo.hpp"

namespace GPlatform {

class GPCORE_API GpTypeInfo
{
public:
	CLASS_DECLARE_DEFAULTS(GpTypeInfo);

	using TypeT			= GpType;
	using TypeTE		= TypeT::EnumT;
	using ContainerT	= GpTypeContainer;
	using ContainerTE	= ContainerT::EnumT;
	using PropT			= GpTypePropInfo;
	using PropsT		= PropT::C::Vec;

public:
						GpTypeInfo			(void) noexcept = default;
						GpTypeInfo			(const GpTypeInfo& aTypeInfo);
						GpTypeInfo			(GpTypeInfo&& aTypeInfo) noexcept;
						~GpTypeInfo			(void) noexcept;

	GpTypeInfo&			operator=			(const GpTypeInfo& aTypeInfo);
	GpTypeInfo&			operator=			(GpTypeInfo&& aTypeInfo) noexcept;

	const GpUUID&		UID					(void) const noexcept {return iUID;}
	const GpUUID&		InheritanceUID		(void) const noexcept {return iInheritanceUID;}
	std::string_view	Name				(void) const noexcept {return iName;}
	TypeTE				Type				(void) const noexcept {return iType;}
	ContainerTE			Container			(void) const noexcept {return iContainer;}
	TypeTE				ContainerKeyType	(void) const noexcept {return iContainerKeyType;}
	const PropsT&		Props				(void) const noexcept {return iProps;}

private:
	GpUUID				iUID;
	GpUUID				iInheritanceUID;
	std::string			iName;
	TypeTE				iType;
	ContainerTE			iContainer;
	TypeTE				iContainerKeyType;
	PropsT				iProps;
};

}//GPlatform

#endif//GP_USE_REFLECTION