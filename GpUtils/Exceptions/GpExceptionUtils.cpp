#include "GpExceptionUtils.hpp"
#include "../Types/Strings/GpStringOps.hpp"

namespace GPlatform {

static constexpr std::string_view GpException_p1()  {return "[Exception]: '"_sv;}
static constexpr std::string_view GpException_p2()  {return "'\n"_sv;}
static constexpr std::string_view GpException_p3()  {return " Function: '"_sv;}
static constexpr std::string_view GpException_p4()  {return "     File: '"_sv;}
static constexpr std::string_view GpException_p5()  {return "     Line: "_sv;}

GpExceptionUtils::ToStrResT GpExceptionUtils::SToString
(
    const std::string_view              aMessage,
    const SourceLocationT&              aSourceLocation,
    const std::optional<std::string>&   aStackTrace
)
{
    std::string_view    p1 = GpException_p1();
    std::string_view    p3 = GpException_p3();
    std::string_view    fileName(reinterpret_cast<const char*>(aSourceLocation.file_name()));
    std::string_view    functioneName(reinterpret_cast<const char*>(aSourceLocation.function_name()));
    const std::string   line(std::to_string(aSourceLocation.line()));

#if defined(GP_OS_WINDOWS)
    constexpr const char PATH_SEPARATOR = '\\';
#else
    constexpr const char PATH_SEPARATOR = '/';
#endif

    fileName = fileName.substr(fileName.find_last_of(PATH_SEPARATOR) + 1);
    //fileName = fileName.substr(0, fileName.find_last_of('.'));

    const size_t plen =
          (std::size(p1)               + std::size(aMessage)      + std::size(GpException_p2()))
        + (std::size(p3)               + std::size(functioneName) + std::size(GpException_p2()))
        + (std::size(GpException_p4()) + std::size(fileName)      + std::size(GpException_p2()))
        + (std::size(GpException_p5()) + std::size(line))
        + (aStackTrace.has_value() ? std::size(aStackTrace.value()) : 0);

    ToStrResT res;

    res.fullMessage.reserve(plen);

    res.fullMessage
        .append(p1).append(aMessage).append(GpException_p2())
        .append(p3).append(functioneName).append(GpException_p2())
        .append(GpException_p4()).append(fileName).append(GpException_p2())
        .append(GpException_p5()).append(line);

    if (aStackTrace.has_value())
    {
        res.fullMessage.append(aStackTrace.value());
    }

    res.message = std::string_view(std::data(res.fullMessage) + std::size(p1), std::size(aMessage));

    return res;
}

}// namespace GPlatform
