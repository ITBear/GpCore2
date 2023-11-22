TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=8124a79f-f6fd-4fa8-6a92-b6727a037ddb
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpTasks
DIR_LEVEL       = ./../..

DEFINES        += GP_TASKS_LIBRARY
DEFINES        += "GP_CURRENT_LIB_VER_MAJ=\\\"$$VER_MAJ\\\""
DEFINES        += "GP_CURRENT_LIB_VER_MIN=\\\"$$VER_MIN\\\""
DEFINES        += "GP_CURRENT_LIB_VER_PAT=\\\"$$VER_PAT\\\""
DEFINES        += "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	Fibers/Boost/GpStackImplPoolBoost.cpp \
	Fibers/Boost/GpTaskFiberCtxBoost.cpp \
	Fibers/Boost/GpTaskFiberCtxFactoryBoost.cpp \
	Fibers/GpTaskFiber.cpp \
	Fibers/GpTaskFiberCtxFactory.cpp \
    Fibers/GpTaskFiberCtxForceUnwind.cpp \
    GpTask.cpp \
    GpTaskEnums.cpp \
    GpTaskPayloadStorage.cpp \
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
    GpTaskPayloadStorage.hpp \
	GpTaskThread.hpp \
    GpTasks_global.hpp \
    ITC/GpItcResult.hpp \
    ITC/GpItcSharedCondition.hpp \
	ITC/GpItcSharedFuture.hpp \
	ITC/GpItcSharedPromise.hpp \
    ITC/GpItcSharedQueue.hpp \
    Scheduler/GpTaskScheduler.hpp \
    Scheduler/GpTaskSchedulerFactory.hpp \
	GpTaskVarStorage.hpp \
	Scheduler/V1/GpTaskExecutorV1.hpp \
	Scheduler/V1/GpTaskSchedulerV1.hpp \
	Scheduler/V1/GpTaskSchedulerV1Factory.hpp
