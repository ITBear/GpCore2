# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=85f5a110-ed86-4537-682f-bfb939507969
PACKET_NAME     = GpUtils
DEFINES        += GP_UTILS_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	#LIBS += -lAdvapi32
}

equals(var_os, "linux") {
	LIBS += -lfmt
}

# ----------- Sources and headers -----------
SOURCES += \
    DateTime/GpDateTimeFormat.cpp \
    DateTime/GpDateTimeOps.cpp \
    Debugging/GpDebugging.cpp \
    Debugging/GpStackTrace.cpp \
    Encoders/GpBase58.cpp \
    Encoders/GpBase64.cpp \
    Exceptions/GpException.cpp \
    Exceptions/GpExceptionCode.cpp \
    Exceptions/GpExceptionTextCode.cpp \
    Exceptions/GpExceptionUtils.cpp \
    Files/GpFile.cpp \
    Files/GpFileMemMap.cpp \
    Files/GpFileUtils.cpp \
    GpUtilsLib.cpp \
    Other/ArgParser/GpArgParser.cpp \
    Other/ArgParser/GpArgParserArgument.cpp \
    Other/ArgParser/GpArgParserArgumentBuilder.cpp \
    Other/ArgParser/GpArgParserRes.cpp \
    Other/GpEmscriptenUtils.cpp \
    Other/GpErrno.cpp \
    Other/GpLinkedLibsInfo.cpp \
    Other/GpStartStopManager.cpp \
    Other/GpSystemInfo.cpp \
    Random/GpRandom.cpp \
    Random/GpRandomDeviceWin.cpp \
    Random/GpRandomIf.cpp \
    Random/GpSRandom.cpp \
    Streams/GpByteReader.cpp \
    Streams/GpByteWriter.cpp \
    Streams/GpByteWriterStorage.cpp \
    Streams/GpByteWriterStorageByteArray.cpp \
    Streams/GpByteWriterStorageFixedSize.cpp \
    Threads/GpThread.cpp \
    Threads/Timers/GpTimer.cpp \
    Threads/Timers/GpTimersManager.cpp \
    Types/Containers/GpGlobalStructCatalogC.cpp \
    Types/Enums/GpEnum.cpp \
    Types/Enums/GpEnumFlags.cpp \
    Types/Strings/GpStringOps.cpp \
    Types/Strings/GpStringUtils.cpp \
    Types/Strings/GpUTF.cpp \
    Types/UIDs/GpUUID.cpp

HEADERS += \
    ../Config/GpCompilerFeatures.hpp \
    ../Config/GpConfig.hpp \
    ../Config/GpConfig_arch_arm_5T.hpp \
    ../Config/GpConfig_arch_arm_7A.hpp \
    ../Config/GpConfig_arch_arm_CORTEX_M.hpp \
    ../Config/GpConfig_arch_mips.hpp \
    ../Config/GpConfig_arch_wasm_32.hpp \
    ../Config/GpConfig_arch_wasm_64.hpp \
    ../Config/GpConfig_arch_x86.hpp \
    ../Config/GpConfig_arch_x86_64.hpp \
    ../Config/GpConfig_os_android.hpp \
    ../Config/GpConfig_os_baremetal.hpp \
    ../Config/GpConfig_os_browser.hpp \
    ../Config/GpConfig_os_ios.hpp \
    ../Config/GpConfig_os_ios_simulator.hpp \
    ../Config/GpConfig_os_linux.hpp \
    ../Config/GpConfig_os_macosx.hpp \
    ../Config/GpConfig_os_windows.hpp \
    ../Config/GpEnvironmentDetector.hpp \
    ../Config/IncludeExt/boost_flat_map.hpp \
    ../Config/IncludeExt/boost_flat_set.hpp \
    ../Config/IncludeExt/boost_small_vector.hpp \
    ../Config/IncludeExt/fmt.hpp \
    ../Config/IncludeExt/windows.hpp \
    Algorithms/GpAverage.hpp \
    Algorithms/GpDistributeProportional.hpp \
    Algorithms/GpFind.hpp \
    Algorithms/GpHysteresis.hpp \
    Algorithms/GpSplit.hpp \
    Concepts/GpConcepts.hpp \
    DateTime/GpDateTimeFormat.hpp \
    DateTime/GpDateTimeOps.hpp \
    Debugging/GpDebugging.hpp \
    Debugging/GpSourceLocation.hpp \
    Debugging/GpStackTrace.hpp \
    Encoders/GpBase58.hpp \
    Encoders/GpBase64.hpp \
    EventBus/Events/GpDataProcessUpdateEvent.hpp \
    EventBus/GpEventChannel.hpp \
    EventBus/GpEventChannelAny.hpp \
    Exceptions/GpException.hpp \
    Exceptions/GpExceptionCe.hpp \
    Exceptions/GpExceptionCode.hpp \
    Exceptions/GpExceptionTextCode.hpp \
    Exceptions/GpExceptionUtils.hpp \
    Exceptions/GpOnThrowStackUnwindFn.hpp \
    Files/GpFile.hpp \
    Files/GpFileMemMap.hpp \
    Files/GpFilePosix.hpp \
    Files/GpFileUtils.hpp \
    Files/GpFileWindows.hpp \
    GpMemOps.hpp \
    GpUtilsLib.hpp \
    GpUtils_global.hpp \
    Macro/GpMacroClass.hpp \
    Macro/GpMacroImportExport.hpp \
    Macro/GpMacroTags.hpp \
    Macro/GpMacroWarnings.hpp \
    Other/ArgParser/GpArgParser.hpp \
    Other/ArgParser/GpArgParserArgument.hpp \
    Other/ArgParser/GpArgParserArgumentBuilder.hpp \
    Other/ArgParser/GpArgParserRes.hpp \
    Other/GpCallHandler.hpp \
    Other/GpCallHandler2.hpp \
    Other/GpCallOnce.hpp \
    Other/GpEmscriptenUtils.hpp \
    Other/GpErrno.hpp \
    Other/GpLinkedLibsInfo.hpp \
    Other/GpMethodAccessGuard.hpp \
    Other/GpRAIIonDestruct.hpp \
    Other/GpStartStopManager.hpp \
    Other/GpSystemInfo.hpp \
    Other/GpSystemInfoWin.hpp \
    Random/GpRandom.hpp \
    Random/GpRandomDeviceIf.hpp \
    Random/GpRandomDeviceWin.hpp \
    Random/GpRandomIf.hpp \
    Random/GpSRandom.hpp \
    Streams/GpByteReader.hpp \
    Streams/GpByteReaderStorage.hpp \
    Streams/GpByteSerializer.hpp \
    Streams/GpByteSerializerFactory.hpp \
    Streams/GpByteWriter.hpp \
    Streams/GpByteWriterRef.hpp \
    Streams/GpByteWriterStorage.hpp \
    Streams/GpByteWriterStorageByteArray.hpp \
    Streams/GpByteWriterStorageFixedSize.hpp \
    SyncPrimitives/GpAsmSpinPause.hpp \
    SyncPrimitives/GpConditionVar.hpp \
    SyncPrimitives/GpConditionVarFlag.hpp \
    SyncPrimitives/GpMutex.hpp \
    SyncPrimitives/GpSharedMutex.hpp \
    SyncPrimitives/GpSpinLock.hpp \
    SyncPrimitives/GpSpinLockRW.hpp \
    SyncPrimitives/GpUnlockGuard.hpp \
    Threads/GpRunnable.hpp \
    Threads/GpSleepStrategy.hpp \
    Threads/GpThread.hpp \
    Threads/GpThreadsSafety.hpp \
    Threads/Timers/GpDoOnceInPeriod.hpp \
    Threads/Timers/GpTimer.hpp \
    Threads/Timers/GpTimersManager.hpp \
    TypeTraits/GpTypeInfoUtils.hpp \
    TypeTraits/GpTypeTraitsArray.hpp \
    TypeTraits/GpTypeTraitsInvocable.hpp \
    TypeTraits/GpTypeTraitsResultOf.hpp \
    TypeTraits/GpTypeTraitsTuple.hpp \
    Types/Bits/GpBitOps.hpp \
    Types/Bool/GpBool.hpp \
    Types/Containers/GpAny.hpp \
    Types/Containers/GpBytesArray.hpp \
    Types/Containers/GpCacheMap.hpp \
    Types/Containers/GpContainersT.hpp \
    Types/Containers/GpDictionary.hpp \
    Types/Containers/GpElementsPool.hpp \
    Types/Containers/GpGlobalStructCatalogC.hpp \
    Types/Containers/GpMultiKeyManager.hpp \
    Types/Containers/GpSet.hpp \
    Types/Containers/GpSharedQueue.hpp \
    Types/Containers/GpSharedQueueSimple.hpp \
    Types/Containers/GpSpanNumerics.hpp \
    Types/Containers/GpTypeShell.hpp \
    Types/Containers/GpVectorWrapper.hpp \
    Types/Enums/GpEnum.hpp \
    Types/Enums/GpEnumFlags.hpp \
    Types/Numerics/GpMath.hpp \
    Types/Numerics/GpNumericOps.hpp \
    Types/Numerics/GpNumericTypes.hpp \
    Types/Pointers/GpReferenceCounter.hpp \
    Types/Pointers/GpReferenceStorage.hpp \
    Types/Pointers/GpSharedPtr.hpp \
    Types/Pointers/GpSpan.hpp \
    Types/Strings/GpStringLiterals.hpp \
    Types/Strings/GpStringOps.hpp \
    Types/Strings/GpStringTemplateArg.hpp \
    Types/Strings/GpStringUtils.hpp \
    Types/Strings/GpUTF.hpp \
    Types/UIDs/GpUUID.hpp \
    Types/Units/GpUnit.hpp \
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
    Types/Units/Other/monetary_t.hpp \
    Types/Units/Other/pixels_t.hpp \
    Types/Units/Other/size_byte_t.hpp \
    Types/Units/Other/size_mebibyte_t.hpp \
    Types/Units/Other/unix_ts_t.hpp \
    Types/Units/SI/GpUnitsSI_Angle.hpp \
    Types/Units/SI/GpUnitsSI_Frequency.hpp \
    Types/Units/SI/GpUnitsSI_Length.hpp \
    Types/Units/SI/GpUnitsSI_Time.hpp \
    Types/Units/SI/GpUnitsSI_Weight.hpp
