TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=2991942d-b5ea-46a0-4b40-c603f8b4e1b7
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
#DEFINES       += _LIBRARY
PACKET_NAME     = Config
DIR_LEVEL       = ./../..

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES +=

HEADERS += \
    GpCompilerFeatures.hpp \
    GpConfig.hpp \
    GpConfig_arch_arm_5T.hpp \
    GpConfig_arch_arm_7A.hpp \
    GpConfig_arch_arm_CORTEX_M.hpp \
    GpConfig_arch_mips.hpp \
    GpConfig_arch_wasm_32.hpp \
    GpConfig_arch_wasm_64.hpp \
    GpConfig_arch_x86.hpp \
    GpConfig_arch_x86_64.hpp \
    GpConfig_os_android.hpp \
    GpConfig_os_baremetal.hpp \
    GpConfig_os_browser.hpp \
    GpConfig_os_ios.hpp \
    GpConfig_os_ios_simulator.hpp \
    GpConfig_os_linux.hpp \
    GpConfig_os_macosx.hpp \
    GpConfig_os_windows.hpp \
    GpEnvironmentDetector.hpp
