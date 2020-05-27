#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "boost_context.hpp"
#include <iostream>

namespace GPlatform {

GpTaskFiber::GpTaskFiber (void) noexcept
{
}

GpTaskFiber::~GpTaskFiber (void) noexcept
{
}

GpTask::Res	GpTaskFiber::Do (GpThreadStopToken aStopToken) noexcept
{
	GpTaskFiber::Res taskRes = GpTaskFiber::Res::DONE;

	try
	{
		switch (iStage.Value())
		{
			case StageT::NOT_RUN:
			{
				iCtx = GpTaskFiberCtx::SP::SNew();
				iCtx.Vn().Init();
				iStage = StageT::RUN;
			} [[fallthrough]];
			case StageT::RUN:
			{
				taskRes = iCtx.Vn().Enter(aStopToken, std::bind(&GpTaskFiber::FiberFn, this, std::placeholders::_1));
			} break;
			case StageT::FINISHED: [[fallthrough]];
			default:
			{
				//NOP
			} break;
		}
	} catch (const std::exception& e)
	{
		iCtx.Clear();
		iStage = StageT::FINISHED;
		GpExceptionsSink::SSink(e);
	} catch (...)
	{
		iCtx.Clear();
		iStage = StageT::FINISHED;
		GpExceptionsSink::SSinkUnknown();
	}

	if (taskRes == GpTask::Res::DONE)
	{
		iStage = StageT::FINISHED;
		iCtx.Clear();
	}

	return taskRes;
}

/*void	GpTaskFiber::FiberFn (GpThreadStopToken aStopToken)
{
	size_t counter = 0;

	A a1;
	counter++;
	std::cout << "[GpTaskFiber::RunNext]: counter = " << counter << std::endl;
	GpTaskFiberCtx::SYeld(GpTask::Res::READY_TO_EXEC);

	while (!aStopToken.stop_requested())
	{
		GpTaskFiberCtx::SYeld(GpTask::Res::READY_TO_EXEC);
		counter++;
	}

	A a2;
	counter++;
	std::cout << "[GpTaskFiber::RunNext]: counter = " << counter << std::endl;
	GpTaskFiberCtx::SYeld(GpTask::Res::DONE);

	A a3;
	counter++;
	std::cout << "[GpTaskFiber::RunNext]: counter = " << counter << std::endl;
}*/

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
