#include "GpExceptionUtils.hpp"
#include "../Types/Strings/GpStringOps.hpp"

namespace GPlatform{

//static constexpr std::u8string_view GpException_p1_std()  {return u8">_< Ouch! [std::exception]: '"_sv;}
//static constexpr std::u8string_view GpException_p1_gp()   {return u8">_< Ouch! [GpException]: '"_sv;}

static constexpr std::u8string_view GpException_p1_std()    {return u8"[std::exception]: '"_sv;}
static constexpr std::u8string_view GpException_p1_gp()     {return u8"[GpException]: '"_sv;}

static constexpr std::u8string_view GpException_p2()        {return u8"'\n"_sv;}

static constexpr std::u8string_view GpException_p3_std()    {return u8" Catch Function: '"_sv;}
static constexpr std::u8string_view GpException_p3_gp()     {return u8"       Function: '"_sv;}

static constexpr std::u8string_view GpException_p4()        {return u8"           File: '"_sv;}
static constexpr std::u8string_view GpException_p5()        {return u8"           Line: "_sv;}

GpExceptionUtils::ToStrResT GpExceptionUtils::SToString
(
    const std::u8string_view        aMessage,
    const SourceLocationT&          aSourceLocation,
    const ExceptionType             aExceptionType,
    std::optional<std::u8string>    aStackTrace
)
{
    std::u8string_view p1;
    std::u8string_view p3;

    if (aExceptionType == ExceptionType::STD)
    {
        p1 = GpException_p1_std();
        p3 = GpException_p3_std();
    } else//aExceptionType == ExceptionType::GP
    {
        p1 = GpException_p1_gp();
        p3 = GpException_p3_gp();
    }

    std::u8string_view  fileName(reinterpret_cast<const char8_t*>(aSourceLocation.file_name()));
    std::u8string_view  functioneName(reinterpret_cast<const char8_t*>(aSourceLocation.function_name()));
    const std::u8string line(StrOps::SToString(aSourceLocation.line()));

    fileName = fileName.substr(fileName.find_last_of('/') + 1);
    fileName = fileName.substr(0, fileName.find_last_of('.'));

    const size_t plen =
          (p1.length()               + aMessage.length()      + GpException_p2().length())
        + (p3.length()               + functioneName.length() + GpException_p2().length())
        + (GpException_p4().length() + fileName.length()      + GpException_p2().length())
        + (GpException_p5().length() + line.length())
        + (aStackTrace.has_value() ? aStackTrace->length() : 0);

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

    res.message = std::u8string_view(res.fullMessage.data() + p1.length(), aMessage.size());

    return res;
}

}//namespace GPlatform
