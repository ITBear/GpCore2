#include "GpTaskFiberManager.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberManager::GpTaskFiberManager (void) noexcept
{
}

GpTaskFiberManager::~GpTaskFiberManager (void) noexcept
{
}

GpTaskFiberManager&	GpTaskFiberManager::S (void)
{
	static GpTaskFiberManager sManager;
	return sManager;
}

void	GpTaskFiberManager::Init (const count_t			aMaxStacksCount,
								  const size_mebibyte_t	aStackSize)
{
	iStackPool.SetStackSize(aStackSize);
	iStackPool.Init(0_cnt, aMaxStacksCount);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)