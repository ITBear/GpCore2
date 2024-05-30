# ----------- Config -----------
TEMPLATE        = lib
#CONFIG        += staticlib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=2991942d-b5ea-46a0-4b40-c603f8b4e1b7
PACKET_NAME     = GpConfig
DEFINES        += GP_CONFIG_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 5
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

# ----------- Libraries -----------
os_windows{
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
	GpConfig.cpp

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
    GpEnvironmentDetector.hpp \
    IncludeExt/boost_flat_map.hpp \
    IncludeExt/boost_flat_set.hpp \
    IncludeExt/boost_small_vector.hpp \
    IncludeExt/fmt.hpp \
    IncludeExt/windows.hpp
