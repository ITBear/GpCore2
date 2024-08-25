# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=8124a79f-f6fd-4fa8-6a92-b6727a037ddb
PACKET_NAME     = GpTasks
DEFINES        += GP_TASKS_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

release_build_static{
	CONFIG += staticlib
}

# ----------- Libraries -----------
os_windows{
	LIBS += -lGpUtils$$TARGET_POSTFIX

	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_fiber-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_fiber-vc143-mt-gd-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:boost_fiber-vc143-mt-gd-x64-1_84
	LIBS +=  -lboost_fiber-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_context-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_context-vc143-mt-gd-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:boost_context-vc143-mt-gd-x64-1_84
	LIBS += -lboost_context-vc143-mt-x64-1_84
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
	Fibers/Boost/GpStackImplPoolBoost.cpp \
	Fibers/Boost/GpTaskFiberCtxBoost.cpp \
	Fibers/Boost/GpTaskFiberCtxFactoryBoost.cpp \
	Fibers/GpTaskFiber.cpp \
	Fibers/GpTaskFiberCtxFactory.cpp \
    Fibers/GpTaskFiberCtxForceUnwind.cpp \
    GpTask.cpp \
    GpTaskEnums.cpp \
	GpTaskGroupsManager.cpp \
	GpTaskLib.cpp \
	GpTaskThread.cpp \
    ITC/GpItcSharedCondition.cpp \
    Scheduler/GpTaskScheduler.cpp \
	GpTaskVarStorage.cpp \
	Scheduler/V1/GpTaskExecutorV1.cpp \
	Scheduler/V1/GpTaskSchedulerV1.cpp \
	Scheduler/V1/GpTaskSchedulerV1Factory.cpp

HEADERS += \
	Fibers/Boost/GpFixedSizeStackBoost.hpp \
	Fibers/Boost/GpStackBoost.hpp \
	Fibers/Boost/GpStackImplPoolBoost.hpp \
	Fibers/Boost/GpTaskFiberCtxBoost.hpp \
	Fibers/Boost/GpTaskFiberCtxFactoryBoost.hpp \
	Fibers/GpTaskFiber.hpp \
	Fibers/GpTaskFiberCtx.hpp \
	Fibers/GpTaskFiberCtxFactory.hpp \
    Fibers/GpTaskFiberCtxForceUnwind.hpp \
    GpTask.hpp \
    GpTaskEnums.hpp \
    GpTaskFactory.hpp \
	GpTaskGroupsManager.hpp \
	GpTaskLib.hpp \
	GpTaskThread.hpp \
    GpTasks_global.hpp \
    ITC/GpItcResult.hpp \
    ITC/GpItcSharedCondition.hpp \
	ITC/GpItcSharedFuture.hpp \
	ITC/GpItcSharedFutureUtils.hpp \
	ITC/GpItcSharedPromise.hpp \
    ITC/GpItcSharedQueue.hpp \
	Scheduler/GpTaskExecutor.hpp \
    Scheduler/GpTaskScheduler.hpp \
    Scheduler/GpTaskSchedulerFactory.hpp \
	GpTaskVarStorage.hpp \
	Scheduler/V1/GpTaskExecutorV1.hpp \
	Scheduler/V1/GpTaskSchedulerV1.hpp \
	Scheduler/V1/GpTaskSchedulerV1Factory.hpp
