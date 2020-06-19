#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GpTaskAccessor
{
    CLASS_REMOVE_CTRS(GpTaskAccessor);

public:
    using StateT	= GpTaskState;
    using StateTE	= StateT::EnumT;

public:
    static StateTE		SState			(GpTask& aTask) noexcept {return aTask.State();}
    static void			SUpdateState	(GpTask& aTask, StateTE aNewState) noexcept {aTask.UpdateState(aNewState);}
    static void			SSetScheduler	(GpTask& aTask, GpTaskScheduler& aScheduler) noexcept {aTask.SetScheduler(aScheduler);}
    static void			SSetWeakPtr		(GpTask& aTask, GpTask::WP aWeakPtr) noexcept {aTask.SetWeakPtr(std::move(aWeakPtr));}
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)