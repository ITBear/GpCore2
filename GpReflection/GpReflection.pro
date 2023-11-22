TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=d2333437-f0d4-42ff-e703-fba6f2e7b8e8
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpReflection
DIR_LEVEL       = ./../..

DEFINES        += GP_REFLECTION_LIBRARY
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
    Builders/GpReflectModelBuilder.cpp \
    Builders/GpReflectPropBuilder.cpp \
    GpReflectContainerType.cpp \
    GpReflectManager.cpp \
    GpReflectModel.cpp \
    GpReflectObject.cpp \
    GpReflectObjectDynamic.cpp \
    GpReflectObjectDynamicUtils.cpp \
    GpReflectProp.cpp \
    GpReflectPropFlags.cpp \
    GpReflectType.cpp \
    GpReflectUtils.cpp \
    GpReflection.cpp \
    Models/GpExceptionDesc.cpp \
    Models/GpReflectDemoDesc.cpp \
    Models/GpReflectModelDesc.cpp \
    Models/GpReflectModelDescUtils.cpp \
    Models/GpReflectPropDesc.cpp \
    Serializers/GpReflectSerializer.cpp

HEADERS += \
    Builders/GpReflectBuilders.hpp \
    Builders/GpReflectModelBuilder.hpp \
    Builders/GpReflectPropBuilder.hpp \
    GpReflectConcepts.hpp \
    GpReflectModelSource.hpp \
    GpReflectModelSourceFactory.hpp \
    GpReflectObjectDynamic.hpp \
    GpReflectObjectDynamicUtils.hpp \
    GpReflectObjectFactory.hpp \
    GpReflectPropGetter.hpp \
    GpReflectVisitor.hpp \
    GpReflection.hpp \
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
    Models/GpReflectDemoDesc.hpp \
    Models/GpReflectModelDesc.hpp \
    Models/GpReflectModelDescUtils.hpp \
    Models/GpReflectModels.hpp \
    Models/GpReflectPropDesc.hpp \
    Serializers/GpReflectSerializer.hpp \
    Serializers/GpReflectSerializerCtx.hpp \
    Serializers/GpReflectSerializerFactory.hpp \
    Serializers/GpReflectSerializers.hpp

