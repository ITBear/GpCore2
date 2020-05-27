#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpMemoryStorageViewR.hpp"
#include "GpMemoryStorageViewRW.hpp"

namespace GPlatform {

class GPCORE_API GpMemoryStorage
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpMemoryStorage);
	CLASS_DECLARE_DEFAULTS(GpMemoryStorage);

public:
	using ViewR		= GpMemoryStorageViewR;
	using ViewRW	= GpMemoryStorageViewRW;

protected:
								GpMemoryStorage		(void) noexcept = default;

public:
	virtual						~GpMemoryStorage	(void) noexcept = default;

	virtual void				Clear				(void) noexcept = 0;
	virtual void				Allocate			(count_t aSize) = 0;
	virtual void				Resize				(count_t aSize) = 0;
	virtual void				Set					(GpMemoryStorage&& aStorage) = 0;
	virtual void				Set					(std::string_view aData) = 0;
	virtual count_t				Size				(void) const noexcept = 0;
	virtual bool				IsEmpty				(void) const noexcept = 0;

	virtual ViewR::SP			ViewRead			(void) const = 0;
	virtual ViewRW::SP			ViewReadWrite		(void) = 0;

	virtual GpMemoryStorage::SP	New					(void) const = 0;
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
