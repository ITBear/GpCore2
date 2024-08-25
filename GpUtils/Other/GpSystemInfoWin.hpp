#pragma once

#include <GpCore2/Config/IncludeExt/windows.hpp>

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

class GpSystemInfoWin
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSystemInfoWin)

public:
    inline static std::string   SWindowsVersion (void);
};

std::string GpSystemInfoWin::SWindowsVersion (void)
{
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");

    if (!hMod)
    {
        return "Failed to get Windows version (ntdll.dll not found)";
    }

    GP_WARNING_PUSH();
#if defined(GP_COMPILER_MSVC)
#   pragma warning(disable : 4191)
#endif// #if defined(GP_COMPILER_MSVC)

    typedef NTSTATUS (WINAPI *RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    RtlGetVersionPtr RtlGetVersionFunc = reinterpret_cast<RtlGetVersionPtr>(GetProcAddress(hMod, "RtlGetVersion"));

    GP_WARNING_POP();

    if (RtlGetVersionFunc == nullptr)
    {
        return "Failed to get Windows version (RtlGetVersion not found)";
    }

    RTL_OSVERSIONINFOW rovi = {sizeof(rovi)};

    if (RtlGetVersionFunc(&rovi) != 0)
    {
        return "Failed to get Windows version";
    }

    std::string osVersion;
    switch (rovi.dwMajorVersion)
    {
        case 10:
        {
            osVersion += "10";
        } break;
        case 6:
        {
            switch (rovi.dwMinorVersion)
            {
                case 1:
                {
                    osVersion += "7";
                } break;
                case 2:
                {
                    osVersion += "8";
                } break;
                case 3:
                {
                    osVersion += "8.1";
                } break;
                default:
                {
                    osVersion += "Unknown";
                };
            }
        } break;
        default:
        {
            osVersion += "Unknown";
        };
    }

    return osVersion;
}

}// namespace GPlatform
