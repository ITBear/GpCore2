#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <atomic>
#include "../../Asm/GpAsmSpinPause.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

namespace GPlatform{

class GpSpinlock
{
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpSpinlock);

public:
			GpSpinlock	(void) noexcept :iState(false) {}
			~GpSpinlock	(void) noexcept = default;

	void	lock		(void) noexcept {while (iState.test_and_set(std::memory_order_acquire)) {GP_ASM_SPIN_PAUSE();}}
	void	unlock		(void) noexcept {iState.clear(std::memory_order_release);}
	bool	try_lock	(void) noexcept {return (iState.test_and_set(std::memory_order_acquire) == false);}

private:
	std::atomic_flag	iState;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
