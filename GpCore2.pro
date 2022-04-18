TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 0
VER_PAT		    = 1
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=96155f96-6bc0-434e-a2da-0f9e72368461
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPCORE_LIBRARY
PACKET_NAME     = GpCore2
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	BOOST_LIB_POSTFIX	= -mgw82-mt-x64-1_75
}

os_linux
{
	#Only if GP_USE_MULTITHREADING_FIBERS set
	LIBS += -lpthread
	LIBS += -lboost_context
	LIBS += -lgmp
	LIBS += -lgmpxx
}

os_browser
{
	LIBS += -lgmp
	LIBS += -lgmpxx
}
#------------------------------ LIBS END ---------------------------------

HEADERS += \
	Algorithms/GpAlgorithms.hpp \
	Algorithms/GpAverage.hpp \
	Algorithms/GpDistributeProportional.hpp \
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
	Constexpr/GpConstexprTuple.hpp \
	Constexpr/GpConstexprUtils.hpp \
	EventBus/GpEvent.hpp \
	EventBus/GpEventPublisher.hpp \
	EventBus/GpEventSubscriber.hpp \
	Exceptions/GpCeExceptions.hpp \
	Exceptions/GpException.hpp \
	Exceptions/GpExceptionDesc.hpp \
	Exceptions/GpExceptionUtils.hpp \
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
	Multithreading/SyncPrimitives/GpUnlockGuard.hpp \
	Multithreading/Tasks/Fibers/GpFixedSizeStack.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiber.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberBarrier.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberBase.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberCtx.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberLambda.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberManager.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStack.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStackPool.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStage.hpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStopEx.hpp \
	Multithreading/Tasks/Fibers/GpTaskFibers.hpp \
	Multithreading/Tasks/Fibers/boost_context.hpp \
	Multithreading/Tasks/GpTask.hpp \
	Multithreading/Tasks/GpTaskAccessor.hpp \
	Multithreading/Tasks/GpTaskBase.hpp \
	Multithreading/Tasks/GpTaskExecutor.hpp \
	Multithreading/Tasks/GpTaskExecutorsPool.hpp \
	Multithreading/Tasks/GpTaskFactory.hpp \
	Multithreading/Tasks/GpTaskLambda.hpp \
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
	RandomGenerators/GpRandomStrMode.hpp \
	RandomGenerators/GpSRandom.hpp \
	Types/Bits/GpBitCast.hpp \
	Types/Bits/GpBitOps.hpp \
	Types/Bits/GpBits.hpp \
	Types/Bits/GpBitset.hpp \
	Types/Bool/GpBool.hpp \
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
	Types/DateTime/GpDateTimeFormat.hpp \
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
	Types/Strings/GpStringTemplateArg.hpp \
	Types/Strings/GpStrings.hpp \
	Types/TypeSystem/GpGlobalStructCatalog.hpp \
	Types/TypeSystem/GpGlobalStructCatalogC.hpp \
	Types/TypeSystem/GpType.hpp \
	Types/TypeSystem/GpTypeContainer.hpp \
	Types/TypeSystem/GpTypeManager.hpp \
	Types/TypeSystem/GpTypeMapper.hpp \
	Types/TypeSystem/GpTypeMapperFactory.hpp \
	Types/TypeSystem/GpTypePropFlags.hpp \
	Types/TypeSystem/GpTypePropInfo.hpp \
	Types/TypeSystem/GpTypePropInfoBuilder.hpp \
	Types/TypeSystem/GpTypePropInfoGetter.hpp \
	Types/TypeSystem/GpTypeStructBase.hpp \
	Types/TypeSystem/GpTypeStructDynamic.hpp \
	Types/TypeSystem/GpTypeStructDynamicBuilder.hpp \
	Types/TypeSystem/GpTypeStructDynamicInfoDesc.hpp \
	Types/TypeSystem/GpTypeStructDynamicPropDesc.hpp \
	Types/TypeSystem/GpTypeStructDynamicUtils.hpp \
	Types/TypeSystem/GpTypeStructFactory.hpp \
	Types/TypeSystem/GpTypeStructInfo.hpp \
	Types/TypeSystem/GpTypeStructVisitor.hpp \
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
	Utils/Encoders/GpBase58.hpp \
	Utils/Encoders/GpBase64.hpp \
	Utils/Encoders/GpEncoders.hpp \
	Utils/Errno/GpErrno.hpp \
	Utils/File/GpFileUtils.hpp \
	Utils/Fn/GpFn.hpp \
	Utils/Fn/GpHysteresis.hpp \
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
	Utils/Streams/GpByteSerializer.hpp \
	Utils/Streams/GpByteSerializerFactory.hpp \
	Utils/Streams/GpByteWriter.hpp \
	Utils/Streams/GpByteWriterStorage.hpp \
	Utils/Streams/GpByteWriterStorageByteArray.hpp \
	Utils/Streams/GpByteWriterStorageFixedSize.hpp \
	Utils/Streams/GpStreams.hpp \
	Utils/Timers/GpDoOnceInPeriod.hpp \
	Utils/Timers/GpMarkTS.hpp \
	Utils/Timers/GpMarkTraceTS.hpp \
	Utils/Timers/GpTimer.hpp \
	Utils/Timers/GpTimerShotEvent.hpp \
	Utils/Timers/GpTimerShotEventFactory.hpp \
	Utils/Timers/GpTimers.hpp \
	Utils/Timers/GpTimersManager.hpp

SOURCES += \
	EventBus/GpEvent.cpp \
	EventBus/GpEventPublisher.cpp \
	EventBus/GpEventSubscriber.cpp \
	Exceptions/GpException.cpp \
	Exceptions/GpExceptionDesc.cpp \
	Exceptions/GpExceptionsSink.cpp \
	Multithreading/SyncPrimitives/GpConditionVar.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiber.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberBarrier.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberBase.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberCtx.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberLambda.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberManager.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStack.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStackPool.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStage.cpp \
	Multithreading/Tasks/Fibers/GpTaskFiberStopEx.cpp \
	Multithreading/Tasks/GpTask.cpp \
	Multithreading/Tasks/GpTaskBase.cpp \
	Multithreading/Tasks/GpTaskExecutor.cpp \
	Multithreading/Tasks/GpTaskExecutorsPool.cpp \
	Multithreading/Tasks/GpTaskLambda.cpp \
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
	RandomGenerators/GpRandomStrMode.cpp \
	RandomGenerators/GpSRandom.cpp \
	Types/DateTime/GpDateTimeFormat.cpp \
	Types/DateTime/GpDateTimeOps.cpp \
	Types/Enums/GpEnum.cpp \
	Types/Enums/GpEnumFlags.cpp \
	Types/Graphs/GpGraph.cpp \
	Types/Graphs/GpGraphEdge.cpp \
	Types/Graphs/GpGraphNode.cpp \
	Types/Numerics/GpNumericOps.cpp \
	Types/Strings/GpStringOps.cpp \
	Types/TypeSystem/GpGlobalStructCatalog.cpp \
	Types/TypeSystem/GpGlobalStructCatalogC.cpp \
	Types/TypeSystem/GpType.cpp \
	Types/TypeSystem/GpTypeContainer.cpp \
	Types/TypeSystem/GpTypeManager.cpp \
	Types/TypeSystem/GpTypeMapper.cpp \
	Types/TypeSystem/GpTypePropFlags.cpp \
	Types/TypeSystem/GpTypePropInfo.cpp \
	Types/TypeSystem/GpTypePropInfoBuilder.cpp \
	Types/TypeSystem/GpTypeStructBase.cpp \
	Types/TypeSystem/GpTypeStructDynamic.cpp \
	Types/TypeSystem/GpTypeStructDynamicBuilder.cpp \
	Types/TypeSystem/GpTypeStructDynamicInfoDesc.cpp \
	Types/TypeSystem/GpTypeStructDynamicPropDesc.cpp \
	Types/TypeSystem/GpTypeStructDynamicUtils.cpp \
	Types/TypeSystem/GpTypeStructInfo.cpp \
	Types/UIDs/GpUUID.cpp \
	UnitTests/GpUnitTest.cpp \
	UnitTests/GpUnitTestCtx.cpp \
	UnitTests/GpUnitTestException.cpp \
	UnitTests/GpUnitTestLayeredCtx.cpp \
	UnitTests/GpUnitTestStreamOutCtx.cpp \
	Utils/Encoders/GpBase58.cpp \
	Utils/Encoders/GpBase64.cpp \
	Utils/Errno/GpErrno.cpp \
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
	Utils/Streams/GpByteWriterStorageFixedSize.cpp \
	Utils/Timers/GpMarkTS.cpp \
	Utils/Timers/GpMarkTraceTS.cpp \
	Utils/Timers/GpTimer.cpp \
	Utils/Timers/GpTimerShotEvent.cpp \
	Utils/Timers/GpTimerShotEventFactory.cpp \
	Utils/Timers/GpTimersManager.cpp

DISTFILES += \
	README.md
