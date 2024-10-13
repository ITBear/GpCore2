#include <GpCore2/GpUtils/Debugging/GpStackTrace.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_PUSH()
    GP_WARNING_DISABLE(shadow)
    GP_WARNING_POP()
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#if defined(GP_POSIX)
#   include <execinfo.h>
#elif defined(GP_OS_WINDOWS) || defined(GP_OS_BROWSER)
#   include <boost/stacktrace/stacktrace.hpp>
#else
#   error Unsupported platform
#endif

namespace GPlatform {

std::string GpStackTrace::STraceToStr (void)
{
    std::string stackTraceStr;
    stackTraceStr.reserve(512);
    stackTraceStr.append("Stack trace:");

#if defined(GP_POSIX)
    void*       ptrs[100];
    const int   size = backtrace(ptrs, 100);

    char** stackTracePtrs = backtrace_symbols(ptrs, size);

    for (int id = 0; id < size; id++)
    {
        stackTraceStr.append
        (
            fmt::format
            (
                "\n    [{}]: {}",
                size - id,
                stackTracePtrs[id]
            )
        );
    }

    std::free(stackTracePtrs);
#elif defined(GP_OS_WINDOWS) || defined(GP_OS_BROWSER)
    stackTraceStr = boost::stacktrace::to_string(boost::stacktrace::stacktrace());
#else
#   error Unsupported platform
#endif//

    return stackTraceStr;
}

void    GpStackTrace::SPrintStacktrace  (std::string_view aMessage)
{
    GpStringUtils::SCout(aMessage + STraceToStr());
}

}// namespace GPlatform
