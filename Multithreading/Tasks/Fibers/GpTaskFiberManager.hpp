#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStackPool.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberManager
{
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskFiberManager);

private:
								GpTaskFiberManager	(void) noexcept;
								~GpTaskFiberManager	(void) noexcept;

public:
	static GpTaskFiberManager&	S					(void);

	void						Init				(const count_t		aMaxStacksCount,
													 const size_byte_t	aStackSize);
	GpTaskFiberStackPool&		StackPool			(void) noexcept {return iStackPool;}

private:
	GpTaskFiberStackPool		iStackPool;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
