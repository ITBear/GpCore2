# ----------- Config -----------
TEMPLATE        = lib
#CONFIG        += staticlib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=d2333437-f0d4-42ff-e703-fba6f2e7b8e8
PACKET_NAME     = GpReflection
DEFINES        += GP_REFLECTION_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 5
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

# ----------- Libraries -----------
os_windows{
	LIBS += -lGpUtils$$TARGET_POSTFIX
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
    Builders/GpReflectModelBuilder.cpp \
    Builders/GpReflectModelPropBuilder.cpp \
    Builders/GpReflectObjectBuilder.cpp \
    Diff/GpReflectDiffDelta.cpp \
    Diff/GpReflectDiffPropBLOB.cpp \
    Diff/GpReflectDiffPropEnum.cpp \
    Diff/GpReflectDiffPropEnumFlags.cpp \
    Diff/GpReflectDiffPropObject.cpp \
    Diff/GpReflectDiffPropObjectSP.cpp \
    Diff/GpReflectDiffPropString.cpp \
    Diff/GpReflectDiffProp_Map.cpp \
    Diff/GpReflectDiffProp_VecBLOB.cpp \
    Diff/GpReflectDiffProp_VecDouble.cpp \
    Diff/GpReflectDiffProp_VecFloat.cpp \
    Diff/GpReflectDiffProp_VecObject.cpp \
    Diff/GpReflectDiffProp_VecObjectSP.cpp \
    Diff/GpReflectDiffProp_VecSI16.cpp \
    Diff/GpReflectDiffProp_VecSI32.cpp \
    Diff/GpReflectDiffProp_VecSI64.cpp \
    Diff/GpReflectDiffProp_VecSI8.cpp \
    Diff/GpReflectDiffProp_VecString.cpp \
    Diff/GpReflectDiffProp_VecUI16.cpp \
    Diff/GpReflectDiffProp_VecUI32.cpp \
    Diff/GpReflectDiffProp_VecUI64.cpp \
    Diff/GpReflectDiffProp_VecUI8.cpp \
    Diff/GpReflectDiffProp_VecUUID.cpp \
    Diff/GpReflectDiffUtils.cpp \
    Diff/GpReflectDiffUtils_CalcDiff.cpp \
    GpReflectContainerType.cpp \
    GpReflectManager.cpp \
    GpReflectModel.cpp \
    GpReflectObject.cpp \
    GpReflectObjectDynamic.cpp \
    GpReflectObjectDynamicUtils.cpp \
    GpReflectProp.cpp \
    GpReflectPropFlags.cpp \
    GpReflectPropUtils.cpp \
    GpReflectType.cpp \
    GpReflectUtils.cpp \
    GpReflectUtils_AssignValue.cpp \
    GpReflectUtils_IsEqual.cpp \
    GpReflectUtils_TotalMemoryUse.cpp \
    GpReflectionLib.cpp \
    Models/GpExceptionDesc.cpp \
    Models/GpReflectModelDesc.cpp \
    Models/GpReflectModelDescUtils.cpp \
    Models/GpReflectPropDesc.cpp \
    Serializers/GpReflectSerializer.cpp

HEADERS += \
    Builders/GpReflectModelBuilder.hpp \
    Builders/GpReflectModelPropBuilder.hpp \
    Builders/GpReflectObjectBuilder.hpp \
    Diff/GpReflectDiffArray.hpp \
    Diff/GpReflectDiffDelta.hpp \
    Diff/GpReflectDiffPropBLOB.hpp \
    Diff/GpReflectDiffPropEnum.hpp \
    Diff/GpReflectDiffPropEnumFlags.hpp \
    Diff/GpReflectDiffPropObject.hpp \
    Diff/GpReflectDiffPropObjectSP.hpp \
    Diff/GpReflectDiffPropString.hpp \
    Diff/GpReflectDiffProp_Map.hpp \
    Diff/GpReflectDiffProp_VecBLOB.hpp \
    Diff/GpReflectDiffProp_VecDouble.hpp \
    Diff/GpReflectDiffProp_VecFloat.hpp \
    Diff/GpReflectDiffProp_VecObject.hpp \
    Diff/GpReflectDiffProp_VecObjectSP.hpp \
    Diff/GpReflectDiffProp_VecSI16.hpp \
    Diff/GpReflectDiffProp_VecSI32.hpp \
    Diff/GpReflectDiffProp_VecSI64.hpp \
    Diff/GpReflectDiffProp_VecSI8.hpp \
    Diff/GpReflectDiffProp_VecString.hpp \
    Diff/GpReflectDiffProp_VecUI16.hpp \
    Diff/GpReflectDiffProp_VecUI32.hpp \
    Diff/GpReflectDiffProp_VecUI64.hpp \
    Diff/GpReflectDiffProp_VecUI8.hpp \
    Diff/GpReflectDiffProp_VecUUID.hpp \
    Diff/GpReflectDiffUtils.hpp \
    Diff/GpReflectDiffUtils_CalcDiff.hpp \
    GpReflectConcepts.hpp \
    GpReflectModelSource.hpp \
    GpReflectModelSourceFactory.hpp \
    GpReflectObjWrapVector.hpp \
    GpReflectObjectDynamic.hpp \
    GpReflectObjectDynamicUtils.hpp \
    GpReflectObjectFactory.hpp \
    GpReflectPropGetter.hpp \
    GpReflectPropUtils.hpp \
    GpReflectUtils_AssignValue.hpp \
    GpReflectUtils_IsEqual.hpp \
    GpReflectUtils_TotalMemoryUse.hpp \
    GpReflectVisitor.hpp \
    GpReflectionLib.hpp \
    GpReflection_global.hpp \
    GpReflectContainerType.hpp \
    GpReflectManager.hpp \
    GpReflectModel.hpp \
    GpReflectObject.hpp \
    GpReflectProp.hpp \
    GpReflectPropFlags.hpp \
    GpReflectType.hpp \
    GpReflectUtils.hpp \
    Models/GpExceptionDesc.hpp \
    Models/GpReflectModelDesc.hpp \
    Models/GpReflectModelDescUtils.hpp \
    Models/GpReflectPropDesc.hpp \
    Serializers/GpReflectSerializer.hpp \
    Serializers/GpReflectSerializerCtx.hpp \
    Serializers/GpReflectSerializerFactory.hpp

