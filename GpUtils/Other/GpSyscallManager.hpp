#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/Types/Pointers/GpSpan.hpp>

#include <string_view>
#include <optional>

#if defined (GP_POSIX)
#   include <sys/types.h>
#endif// #if defined (GP_POSIX)

#if defined (GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined (GP_OS_WINDOWS)

namespace GPlatform {

class GP_UTILS_API GpSyscallManager
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSyscallManager)

    public:
#if defined (GP_POSIX)
    using HandlerT = pid_t;
#elif defined (GP_OS_WINDOWS)
    using HandlerT = PROCESS_INFORMATION;
#else
#   error Unsupported OS
#endif

public:
    static HandlerT             SSpawnProcess           (std::string_view           aCommandToCall,
                                                         const GpSpan<std::string>& aArguments);
    static std::optional<int>   STryGetProcessResult    (HandlerT   aHandler);
    static void                 SSendTerminate          (HandlerT   aHandler);
    static void                 SSendKill               (HandlerT   aHandler);
};

}// namespace GPlatform

#if defined (GP_OS_WINDOWS)
    inline bool operator ==
    (
        const ::GPlatform::GpSyscallManager::HandlerT& lhs,
        const ::GPlatform::GpSyscallManager::HandlerT& rhs
    )
    {
        return lhs.dwProcessId == rhs.dwProcessId;
    }

// ********************** fmt *********************
namespace FMT_NAMESPASE {

    template<>
    struct formatter<::GPlatform::GpSyscallManager::HandlerT>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& aCtx)
        {
            return std::begin(aCtx);
        }

        template<typename FormatContext>
        auto format(const ::GPlatform::GpSyscallManager::HandlerT& aHandler, FormatContext& aCtx) const
        {
            return ::fmt::format_to(aCtx.out(), "{}", aHandler.dwProcessId);
        }
    };

}// namespace FMT_NAMESPASE
#endif// #if defined (GP_OS_WINDOWS)


