CONFIG		-= qt
QT			-= core gui widgets
TEMPLATE	= lib
VER_MAJ		= 2
VER_MIN		= 0
VER_PAT		= 1
CONFIG		+= warn_on
DEFINES		+= GPCORE_LIBRARY \
			HAVE_NETINET_IN_H \
			_GLIBCXX_USE_NANOSLEEP

QMAKE_CXXFLAGS += -DGP_MODULE_UUID=96155f96-6bc0-434e-a2da-0f9e72368461
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER

PACKET_NAME     = GpCore2
OUT_BUILD_PATH  = ./../../../Bin_tmp/

compiler_gcc{
	QMAKE_CC		=	gcc-10
	QMAKE_CXX		=	g++-10
	QMAKE_LINK		=	g++-10
}else:compiler_clang{
	QMAKE_CXXFLAGS	+= -stdlib=libc++
	QMAKE_LFLAGS    += -stdlib=libc++

	#QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	#QMAKE_LFLAGS   += -stdlib=libstdc++
}else:compiler_emscripten{
}else{
	error(Unknown compiler mode. Set CONFIG+=compiler_gcc OR CONFIG+=compiler_clang OR CONFIG+=compiler_emscripten)
}

#c++20
CONFIG			+=	c++2a
QMAKE_CXXFLAGS	+=	-std=gnu++2a

QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_CXXFLAGS	+= -ffunction-sections -fdata-sections -fexceptions -fstrict-aliasing -fstack-clash-protection
QMAKE_CXXFLAGS	+= -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized
QMAKE_CXXFLAGS	+= -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op
QMAKE_CXXFLAGS	+= -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate
QMAKE_CXXFLAGS	+= -Wno-unknown-warning-option -Wno-unused-command-line-argument -Wno-comment
#QMAKE_CXXFLAGS	+= -fno-rtti
QMAKE_LFLAGS    += -Wl,--gc-sections

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
		#BOOST_POSTFIX = _asan
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
LIBS += -L$$DESTDIR

os_windows{
	BOOST_LIB_POSTFIX	= -mgw82-mt-x64-1_72
}

os_linux
{
	#Only if GP_USE_MULTITHREADING_FIBERS set
	LIBS += -lpthread
	LIBS += -lboost_context
}

#------------------------------ LIBS END ---------------------------------

INCLUDEPATH += \
	../Extras \
	../Extras/Boost/boost_1_72_0$$BOOST_POSTFIX

HEADERS += \
	Algorithms/GpAlgorithms.hpp \
	Algorithms/GpAverage.hpp \
	Algorithms/GpFind.hpp \
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
	Constexpr/GpConstexprArray.hpp \
	Constexpr/GpConstexprFalse.hpp \
	Constexpr/GpConstexprIterator.hpp \
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
	Multithreading/Tasks/Fibers/GpBaseTaskFiber.hpp \
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
	Multithreading/Tasks/SyncPrimitives/GpTaskFiberBarrier.hpp \
	Multithreading/Tasks/SyncPrimitives/GpTaskSyncPrimitives.hpp \
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
	Types/Bits/GpBitCast.hpp \
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
	Types/Containers/GpRawPtrByte.hpp \
	Types/Containers/GpRawPtrNumerics.hpp \
	Types/Containers/GpTypeShell.hpp \
	Types/DateTime/GpDateTime.hpp \
	Types/DateTime/GpDateTimeOps.hpp \
	Types/Enums/GpEnum.hpp \
	Types/Enums/GpEnumFlags.hpp \
	Types/Enums/GpEnums.hpp \
	Types/GpTypes.hpp \
	Types/Graphs/DAG/GpGraphDAG.hpp \
	Types/Graphs/GpGraph.hpp \
	Types/Graphs/GpGraphEdge.hpp \
	Types/Graphs/GpGraphNode.hpp \
	Types/Graphs/GpGraphs.hpp \
	Types/Numerics/GpNumericOps.hpp \
	Types/Numerics/GpNumericTypes.hpp \
	Types/Numerics/GpNumerics.hpp \
	Types/Pointers/GpPointers.hpp \
	Types/Pointers/GpRawPtr.hpp \
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
	Types/TypeSystem/GpTypeManager.hpp \
	Types/TypeSystem/GpTypePropInfo.hpp \
	Types/TypeSystem/GpTypePropInfoGetter.hpp \
	Types/TypeSystem/GpTypeStructBase.hpp \
	Types/TypeSystem/GpTypeStructFactory.hpp \
	Types/TypeSystem/GpTypeStructInfo.hpp \
	Types/TypeSystem/GpTypeSystem.hpp \
	Types/TypeSystem/GpTypeUtils.hpp \
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
	Types/Units/Numerics/GpUnitsNumerics_SSizeT.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt16.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt32.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt64.hpp \
	Types/Units/Numerics/GpUnitsNumerics_UInt8.hpp \
	Types/Units/Numerics/GpUnitsNumerics_USizeT.hpp \
	Types/Units/Other/GpOtherUnits.hpp \
	Types/Units/Other/count_t.hpp \
	Types/Units/Other/monetary_t.hpp \
	Types/Units/Other/pixels_t.hpp \
	Types/Units/Other/size_byte_t.hpp \
	Types/Units/Other/size_mebibyte_t.hpp \
	Types/Units/Other/unix_ts_t.hpp \
	Types/Units/SI/GpUnitsSI.hpp \
	Types/Units/SI/GpUnitsSI_Frequency.hpp \
	Types/Units/SI/GpUnitsSI_Length.hpp \
	Types/Units/SI/GpUnitsSI_Time.hpp \
	Types/Units/SI/GpUnitsSI_Weight.hpp \
	UnitTests/GpUnitTest.hpp \
	UnitTests/GpUnitTestCtx.hpp \
	UnitTests/GpUnitTestException.hpp \
	UnitTests/GpUnitTestLayeredCtx.hpp \
	UnitTests/GpUnitTestStreamOutCtx.hpp \
	UnitTests/GpUnitTests.hpp \
	Utils/File/GpFileUtils.hpp \
	Utils/GpUtils.hpp \
	Utils/Pragma/GpPragmaUtils.hpp \
	Utils/RAII/GpOnThrowStackUnwindFn.hpp \
	Utils/RAII/GpRAIIonDestruct.hpp \
	Utils/RAII/GpRAIIutils.hpp \
	Utils/Streams/GpBitReader.hpp \
	Utils/Streams/GpBitReaderStorage.hpp \
	Utils/Streams/GpBitWriter.hpp \
	Utils/Streams/GpBitWriterStorage.hpp \
	Utils/Streams/GpBitWriterStorageByteArray.hpp \
	Utils/Streams/GpBitWriterStorageFixedSize.hpp \
	Utils/Streams/GpByteReader.hpp \
	Utils/Streams/GpByteReaderStorage.hpp \
	Utils/Streams/GpByteWriter.hpp \
	Utils/Streams/GpByteWriterStorage.hpp \
	Utils/Streams/GpByteWriterStorageByteArray.hpp \
	Utils/Streams/GpByteWriterStorageFixedSize.hpp \
	Utils/Streams/GpStreams.hpp

SOURCES += \
	EventBus/GpEventBus.cpp \
	EventBus/GpEventSubscriber.cpp \
	Exceptions/GpException.cpp \
	Exceptions/GpExceptionsSink.cpp \
	Multithreading/SyncPrimitives/GpConditionVar.cpp \
	Multithreading/Tasks/Fibers/GpBaseTaskFiber.cpp \
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
	Multithreading/Tasks/SyncPrimitives/GpTaskFiberBarrier.cpp \
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
	Types/Graphs/GpGraph.cpp \
	Types/Graphs/GpGraphEdge.cpp \
	Types/Graphs/GpGraphNode.cpp \
	Types/Numerics/GpNumericOps.cpp \
	Types/Pointers/GpRawPtr.cpp \
	Types/Strings/GpStringOps.cpp \
	Types/TypeSystem/GpType.cpp \
	Types/TypeSystem/GpTypeContainer.cpp \
	Types/TypeSystem/GpTypeManager.cpp \
	Types/TypeSystem/GpTypePropInfo.cpp \
	Types/TypeSystem/GpTypeStructBase.cpp \
	Types/TypeSystem/GpTypeStructInfo.cpp \
	Types/UIDs/GpUUID.cpp \
	UnitTests/GpUnitTest.cpp \
	UnitTests/GpUnitTestCtx.cpp \
	UnitTests/GpUnitTestException.cpp \
	UnitTests/GpUnitTestLayeredCtx.cpp \
	UnitTests/GpUnitTestStreamOutCtx.cpp \
	Utils/File/GpFileUtils.cpp \
	Utils/Streams/GpBitReader.cpp \
	Utils/Streams/GpBitReaderStorage.cpp \
	Utils/Streams/GpBitWriter.cpp \
	Utils/Streams/GpBitWriterStorage.cpp \
	Utils/Streams/GpBitWriterStorageByteArray.cpp \
	Utils/Streams/GpBitWriterStorageFixedSize.cpp \
	Utils/Streams/GpByteReader.cpp \
	Utils/Streams/GpByteReaderStorage.cpp \
	Utils/Streams/GpByteWriter.cpp \
	Utils/Streams/GpByteWriterStorage.cpp \
	Utils/Streams/GpByteWriterStorageByteArray.cpp \
	Utils/Streams/GpByteWriterStorageFixedSize.cpp
