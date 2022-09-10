TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=8124a79f-f6fd-4fa8-6a92-b6727a037ddb
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES        += GP_TASKS_LIBRARY
PACKET_NAME     = GpTasks
DIR_LEVEL       = ./../..

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    Fibers/GpTaskFiber.cpp \
    Fibers/GpTaskFiberBase.cpp \
    Fibers/GpTaskFiberCtx.cpp \
    Fibers/GpTaskFiberLambda.cpp \
    Fibers/GpTaskFiberManager.cpp \
    Fibers/GpTaskFiberManagerEx.cpp \
    GpTask.cpp \
    GpTaskBase.cpp \
    GpTaskExecutor.cpp \
    GpTaskExecutorsPool.cpp \
    GpTaskLambda.cpp \
    GpTaskScheduler.cpp \
    GpTaskState.cpp \
    ITC/GpItcFlag.cpp \
    ITC/GpItcFuture.cpp

HEADERS += \
    Fibers/GpFixedSizeStack.hpp \
    Fibers/GpTaskFiber.hpp \
    Fibers/GpTaskFiberAccessor.hpp \
    Fibers/GpTaskFiberBase.hpp \
    Fibers/GpTaskFiberCtx.hpp \
    Fibers/GpTaskFiberLambda.hpp \
    Fibers/GpTaskFiberManager.hpp \
    Fibers/GpTaskFiberManagerEx.hpp \
    Fibers/GpTaskFiberStack.hpp \
    Fibers/GpTaskFiberStackPool.hpp \
    Fibers/GpTaskFiberStopEx.hpp \
    Fibers/GpTaskFibers.hpp \
    Fibers/boost_context.hpp \
    GpTask.hpp \
    GpTaskAccessor.hpp \
    GpTaskBase.hpp \
    GpTaskExecutor.hpp \
    GpTaskExecutorsPool.hpp \
    GpTaskFactory.hpp \
    GpTaskLambda.hpp \
    GpTaskScheduler.hpp \
    GpTaskState.hpp \
    GpTasks.hpp \
    GpTasks_global.hpp \
    ITC/GpITCs.hpp \
    ITC/GpItcConsumer.hpp \
    ITC/GpItcFlag.hpp \
    ITC/GpItcFuture.hpp \
    ITC/GpItcProducer.hpp \
    ITC/GpItcProducerConsumer.hpp \
    ITC/GpItcPromise.hpp \
    ITC/GpItcResult.hpp
