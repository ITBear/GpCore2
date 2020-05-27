CONFIG		-= qt
QT			-= core gui widgets
TEMPLATE	= lib
VER_MAJ		= 2
VER_MIN		= 0
VER_PAT		= 0
CONFIG		+= warn_on
DEFINES		+= GPCORE_LIBRARY \
			HAVE_NETINET_IN_H \
			_GLIBCXX_USE_NANOSLEEP

PACKET_NAME     = GpCore
OUT_BUILD_PATH  = ./../../../Bin_tmp/

compiler_gcc{
	os_linux{
		QMAKE_CC	= gcc-9
		QMAKE_CXX	= g++-9
	}
}else:compiler_clang{
	QMAKE_CXXFLAGS	+= -stdlib=libc++
	QMAKE_LFLAGS    += -stdlib=libc++

	#QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	#QMAKE_LFLAGS   += -stdlib=libstdc++
}else:compiler_emscripten{

}else{
	error(Unknown compiler mode. Set CONFIG+=compiler_gcc OR CONFIG+=compiler_clang OR CONFIG+=compiler_emscripten)
}

#c++2a
CONFIG					+=	c++2a
QMAKE_CXXFLAGS_GNUCXX11 =	-std=gnu++2a
QMAKE_CXXFLAGS_GNUCXX14 =	-std=gnu++2a
QMAKE_CXXFLAGS_GNUCXX1Z =	-std=gnu++2a
QMAKE_CXXFLAGS			+=	-std=gnu++2a

compiler_gcc{
	QMAKE_CXXFLAGS	+= -fstrict-aliasing -Wall -Wextra -Wno-comment -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op
	QMAKE_CXXFLAGS	+= -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate
	#QMAKE_CXXFLAGS	+= -fconcepts -fgnu-tm
	QMAKE_CXXFLAGS  += -fstack-clash-protection
}else:compiler_clang{
}else:compiler_emscripten{
}

QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_CXXFLAGS	+= -ffunction-sections -fdata-sections
#QMAKE_CXXFLAGS	+= -fno-rtti
QMAKE_LFLAGS    += -Wl,--gc-sections

QMAKE_CFLAGS	+= -fstrict-aliasing -Wall -Wextra -Wno-comment -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op
QMAKE_CFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden

#------------------------ DEBUG or RELEASE ---------------------
debug_build {
	message([$$PACKET_NAME]: ***************** Build mode DEBUG *****************)
	DEFINES			   += DEBUG_BUILD
	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Debug
	os_linux {
		QMAKE_CXXFLAGS	+= -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr
		LIBS += -lasan
		LIBS += -lubsan
		BOOST_POSTFIX = _asan
	}
} else:release_build {
	message([$$PACKET_NAME]: ***************** Build mode RELEASE *****************)
	DEFINES			    += RELEASE_BUILD
	TARGET_POSTFIX		=
	OUT_BUILD_MODE_PATH	= Release
} else {
	error(Unknown build mode. Set CONFIG+=debug_build OR CONFIG+=release_build)
}

#------------------------ OS ---------------------
os_linux {
	OUT_BUILD_OS_PATH = Linux
} else:os_android {
	OUT_BUILD_OS_PATH = Android
} else:os_ios {
	OUT_BUILD_OS_PATH = Ios
} else:os_windows {
	OUT_BUILD_OS_PATH = Win
} else:os_macx {
	OUT_BUILD_OS_PATH = Macx
} else:os_browser {
	OUT_BUILD_OS_PATH = Browser
} else {
	error(Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser)
}

#------------------------ ARC ---------------------
arc_x86_64 {
	OUT_BUILD_ARCH_PATH = x86_64
	QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64
} else:arc_x86 {
	OUT_BUILD_ARCH_PATH = x86
	QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
} else:arc_arm_v6 {
	OUT_BUILD_ARCH_PATH = arm_v6
} else:arc_armeabi_v7a {
	OUT_BUILD_ARCH_PATH = armeabi_v7a
} else:arc_arm64_v8a {
	OUT_BUILD_ARCH_PATH = arm64_v8a
} else:arc_mips {
	OUT_BUILD_ARCH_PATH = mips
} else:arc_wasm32 {
	OUT_BUILD_ARCH_PATH = wasm32
} else:arc_wasm64 {
	OUT_BUILD_ARCH_PATH = wasm64
} else {
	error(Unknown ARC. Set CONFIG+=... one of values: arc_x86_64, arc_x86, arc_arm_v6, arc_armeabi_v7a, arc_arm64_v8a, arc_mips, arc_wasm32, arc_wasm64)
}

TARGET = $$PACKET_NAME$$TARGET_POSTFIX

SP = _
DESTDIR = $$OUT_BUILD_PATH$$OUT_BUILD_MODE_PATH$$SP$$OUT_BUILD_OS_PATH$$SP$$OUT_BUILD_ARCH_PATH/

message([$$PACKET_NAME]: Target name: $$TARGET)
message([$$PACKET_NAME]: Target arch: $$OUT_BUILD_ARCH_PATH)
message([$$PACKET_NAME]: Target OS:   $$OUT_BUILD_OS_PATH)
message([$$PACKET_NAME]: Build directory $$DESTDIR)
message([$$PACKET_NAME]: -------------------------------------------------)

#------------------------------ LIBS BEGIN ---------------------------------
LIBS += -L$$DESTDIR \
		-L$$DESTDIR/Plugins

os_linux
{
	LIBS += -L/usr/lib/gcc/x86_64-linux-gnu/9
}

os_windows{
	BOOST_LIB_POSTFIX		= -mgw82-mt-x64-1_72
	GPLATFORM_VERSION_LIB	= 2
}

#Only if GP_USE_MULTITHREADING_FIBERS set
LIBS += -lpthread
LIBS += -lboost_context

#------------------------------ LIBS END ---------------------------------

INCLUDEPATH += \
	../../Extras \
	../../Extras/Boost/boost_1_72_0$$BOOST_POSTFIX

HEADERS += \
	Algorithms/GpAlgorithms.hpp \
	Algorithms/GpAverage.hpp \
	Algorithms/GpSplit.hpp \
	Asm/GpAsm.hpp \
	Asm/GpAsmSpinPause.hpp \
	Config/GpCompilerFeatures.hpp \
	Config/GpConfig.hpp \
	Config/GpConfig_arch_arm_5T.hpp \
	Config/GpConfig_arch_arm_7A.hpp \
	Config/GpConfig_arch_arm_CORTEX_M.hpp \
	Config/GpConfig_arch_mips.hpp \
	Config/GpConfig_arch_wasm_32.hpp \
	Config/GpConfig_arch_wasm_64.hpp \
	Config/GpConfig_arch_x86.hpp \
	Config/GpConfig_arch_x86_64.hpp \
	Config/GpConfig_os_android.hpp \
	Config/GpConfig_os_baremetal.hpp \
	Config/GpConfig_os_browser.hpp \
	Config/GpConfig_os_ios.hpp \
	Config/GpConfig_os_ios_simulator.hpp \
	Config/GpConfig_os_linux.hpp \
	Config/GpConfig_os_macosx.hpp \
	Config/GpConfig_os_windows.hpp \
	Config/GpEnvironmentDetector.hpp \
	Constexpr/GpConstexprFalse.hpp \
	Constexpr/GpConstexprUtils.hpp \
	EventBus/GpEvent.hpp \
	EventBus/GpEventBus.hpp \
	EventBus/GpEventSubscriber.hpp \
	Exceptions/GpCeExceptions.hpp \
	Exceptions/GpException.hpp \
	Exceptions/GpExceptions.hpp \
	Exceptions/GpExceptionsSink.hpp \
	GpCore.hpp \
	GpCore_global.hpp \
	Memory/GpMemOps.hpp \
	Memory/GpMemory.hpp \
	Multithreading/GpMultithreading.hpp \
	Multithreading/SyncPrimitives/GpConditionVar.hpp \
	Multithreading/SyncPrimitives/GpRWLock.hpp \
	Multithreading/SyncPrimitives/GpSpinlock.hpp \
	Multithreading/SyncPrimitives/GpSyncPrimitives.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiber.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberCtx.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberManager.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStack.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStackPool.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStage.hpp \
	Multithreading/Tasks/Fibers/GpTaskFibers.hpp \
	Multithreading/Tasks/Fibers/boost_context.hpp \
	Multithreading/Tasks/GpTask.hpp \
	Multithreading/Tasks/GpTaskAccessor.hpp \
	Multithreading/Tasks/GpTaskExecutor.hpp \
	Multithreading/Tasks/GpTaskExecutorsPool.hpp \
	Multithreading/Tasks/GpTaskScheduler.hpp \
	Multithreading/Tasks/GpTaskSchedulerAccessor.hpp \
	Multithreading/Tasks/GpTaskState.hpp \
	Multithreading/Tasks/GpTasks.hpp \
	Multithreading/Threads/GpRunnable.hpp \
	Multithreading/Threads/GpThread.hpp \
	Multithreading/Threads/GpThreadStopToken.hpp \
	Multithreading/Threads/GpThreads.hpp \
	Pipeline/GpPipeline.hpp \
	Pipeline/GpPipelineEvaluateCtx.hpp \
	Pipeline/GpPipelineEvaluator.hpp \
	Pipeline/GpPipelineManager.hpp \
	Pipeline/GpPipelineNode.hpp \
	Pipeline/GpPipelineNodeConnector.hpp \
	Pipeline/GpPipelineNodeSocket.hpp \
	Pipeline/GpPipelineNodeSocketDir.hpp \
	Pipeline/Nodes/GpPipelineNodeSum.hpp \
	Pipeline/Nodes/GpPipelineNodeSumEvCtx.hpp \
	Pipeline/Nodes/GpPipelineNodes.hpp \
	RandomGenerators/GpRandom.hpp \
	RandomGenerators/GpRandomDeviceIf.hpp \
	RandomGenerators/GpRandomDeviceWin.hpp \
	RandomGenerators/GpRandomGenerators.hpp \
	RandomGenerators/GpSRandom.hpp \
	Types/Bits/GpBitOps.hpp \
	Types/Bits/GpBits.hpp \
	Types/Bits/GpBitset.hpp \
	Types/Classes/GpClasses.hpp \
	Types/Classes/GpClassesDefines.hpp \
	Types/Containers/GpBytesArray.hpp \
	Types/Containers/GpContainers.hpp \
	Types/Containers/GpContainersT.hpp \
	Types/Containers/GpElementsCatalog.hpp \
	Types/Containers/GpElementsPool.hpp \
	Types/Containers/GpMemoryStorage.hpp \
	Types/Containers/GpMemoryStorageViewR.hpp \
	Types/Containers/GpMemoryStorageViewRW.hpp \
	Types/Containers/GpTypeShell.hpp \
	Types/DateTime/GpDateTime.hpp \
	Types/DateTime/GpDateTimeOps.hpp \
	Types/Enums/GpEnum.hpp \
	Types/Enums/GpEnumFlags.hpp \
	Types/Enums/GpEnums.hpp \
	Types/GpTypes.hpp \
	Types/Numerics/GpNumericOps.hpp \
	Types/Numerics/GpNumericTypes.hpp \
	Types/Numerics/GpNumerics.hpp \
	Types/Pointers/GpPointers.hpp \
	Types/Pointers/GpReferenceCounter.hpp \
	Types/Pointers/GpReferenceStorage.hpp \
	Types/Pointers/GpSharedPtr.hpp \
	Types/Strings/GpStringLiterals.hpp \
	Types/Strings/GpStringOps.hpp \
	Types/Strings/GpStringOpsGlob.hpp \
	Types/Strings/GpStringTemplateArg.hpp \
	Types/Strings/GpStrings.hpp \
	Types/TypeSystem/GpType.hpp \
	Types/TypeSystem/GpTypeContainer.hpp \
	Types/TypeSystem/GpTypeInfo.hpp \
	Types/TypeSystem/GpTypePropInfo.hpp \
	Types/TypeSystem/GpTypeSystem.hpp \
	Types/TypeSystem/GpTypesManager.hpp \
	Types/UIDs/GpUIDs.hpp \
	Types/UIDs/GpUUID.hpp \
	Types/Units/GpUnit.hpp \
	Types/Units/GpUnitUtils.hpp \
	Types/Units/GpUnits.hpp \
	Types/Units/Numerics/GpUnitsNumerics.hpp \
	Types/Units/Numerics/GpUnitsNumerics_Double.hpp \
	Types/Units/Numerics/GpUnitsNumerics_Float.hpp \
	Types/Units/Numerics/GpUnitsNumerics_SInt16.hpp \
	Types/Units/Numerics/GpUnitsNumerics_SInt32.hpp \
	Types/Units/Numerics/GpUnitsNumerics_SInt64.hpp \
	Types/Units/Numerics/GpUnitsNumerics_SInt8.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt16.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt32.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt64.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt8.hpp \
	Types/Units/Other/GpOtherUnits.hpp \
	Types/Units/Other/count_t.hpp \
	Types/Units/Other/monetary_t.hpp \
	Types/Units/Other/size_byte_t.hpp \
	Types/Units/Other/size_mebibyte_t.hpp \
	Types/Units/Other/unix_ts_t.hpp \
	Types/Units/SI/GpUnitsSI.hpp \
	Types/Units/SI/GpUnitsSI_Frequency.hpp \
	Types/Units/SI/GpUnitsSI_Length.hpp \
	Types/Units/SI/GpUnitsSI_Time.hpp \
	Types/Units/SI/GpUnitsSI_Weight.hpp \
	Utils/GpUtils.hpp \
	Utils/Streams/GpBitIStream.hpp \
	Utils/Streams/GpBitOStream.hpp \
	Utils/Streams/GpBitOStreamStorages.hpp \
	Utils/Streams/GpByteOStream.hpp \
	Utils/Streams/GpByteOStreamStorages.hpp \
	Utils/Streams/GpStreams.hpp

SOURCES += \
	EventBus/GpEventBus.cpp \
	EventBus/GpEventSubscriber.cpp \
	Exceptions/GpException.cpp \
	Exceptions/GpExceptionsSink.cpp \
	Multithreading/SyncPrimitives/GpConditionVar.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiber.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberCtx.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberManager.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStack.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStackPool.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStage.cpp \
	Multithreading/Tasks/GpTask.cpp \
	Multithreading/Tasks/GpTaskExecutor.cpp \
	Multithreading/Tasks/GpTaskExecutorsPool.cpp \
	Multithreading/Tasks/GpTaskScheduler.cpp \
	Multithreading/Tasks/GpTaskState.cpp \
	Multithreading/Threads/GpRunnable.cpp \
	Multithreading/Threads/GpThread.cpp \
	Pipeline/GpPipeline.cpp \
	Pipeline/GpPipelineEvaluateCtx.cpp \
	Pipeline/GpPipelineEvaluator.cpp \
	Pipeline/GpPipelineManager.cpp \
	Pipeline/GpPipelineNode.cpp \
	Pipeline/GpPipelineNodeConnector.cpp \
	Pipeline/GpPipelineNodeSocket.cpp \
	Pipeline/GpPipelineNodeSocketDir.cpp \
	Pipeline/Nodes/GpPipelineNodeSum.cpp \
	Pipeline/Nodes/GpPipelineNodeSumEvCtx.cpp \
	RandomGenerators/GpRandom.cpp \
	RandomGenerators/GpRandomDeviceWin.cpp \
	RandomGenerators/GpSRandom.cpp \
	Types/DateTime/GpDateTimeOps.cpp \
	Types/Enums/GpEnum.cpp \
	Types/Numerics/GpNumericOps.cpp \
	Types/Strings/GpStringOps.cpp \
	Types/TypeSystem/GpType.cpp \
	Types/TypeSystem/GpTypeContainer.cpp \
	Types/TypeSystem/GpTypeInfo.cpp \
	Types/TypeSystem/GpTypePropInfo.cpp \
	Types/TypeSystem/GpTypesManager.cpp \
	Types/UIDs/GpUUID.cpp \
	Utils/Streams/GpBitIStream.cpp \
	Utils/Streams/GpBitOStreamStorages.cpp \
	Utils/Streams/GpByteOStreamStorages.cpp
