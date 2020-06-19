#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../GpCore_global.hpp"
#include "../../../Types/Classes/GpClassesDefines.hpp"
#include "../../../Types/Numerics/GpNumerics.hpp"
#include "../../../Types/Containers/GpContainersT.hpp"
#include "../../../Types/Units/Other/size_byte_t.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberStack
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskFiberStack);
	CLASS_DECLARE_DEFAULTS(GpTaskFiberStack);

public:
						GpTaskFiberStack	(void) noexcept;
						~GpTaskFiberStack	(void) noexcept;

	void				Init				(const size_byte_t aSize);
	void				Clear				(void) noexcept;

	void*				Context				(void) noexcept {return iStackContext;}

private:
	void*				iStackAllocator = nullptr;
	void*				iStackContext	= nullptr;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
