#include "GpExceptionUtils.hpp"

namespace GPlatform{

static constexpr std::string_view GpException_p1_std()  {return ">_< Ouch! [std::exception]: '"_sv;}
static constexpr std::string_view GpException_p1_gp()   {return ">_< Ouch! [GpException]: '"_sv;}

static constexpr std::string_view GpException_p2()      {return "'\n"_sv;}

static constexpr std::string_view GpException_p3_std()  {return " Catch Function: '"_sv;}
static constexpr std::string_view GpException_p3_gp()   {return "       Function: '"_sv;}

static constexpr std::string_view GpException_p4()      {return "           File: '"_sv;}
static constexpr std::string_view GpException_p5()      {return "           Line: "_sv;}

GpExceptionUtils::ToStrResT GpExceptionUtils::SToString
(
    const std::string_view  aMessage,
    const SourceLocationT&  aSourceLocation,
    const ExceptionType     aExceptionType
)
{
    std::string_view p1;
    std::string_view p3;

    if (aExceptionType == ExceptionType::STD)
    {
        p1 = GpException_p1_std();
        p3 = GpException_p3_std();
    } else//aExceptionType == ExceptionType::GP
    {
        p1 = GpException_p1_gp();
        p3 = GpException_p3_gp();
    }

    std::string_view    fileName(aSourceLocation.file_name());
    std::string_view    functioneName(aSourceLocation.function_name());
    const std::string   line(std::to_string(aSourceLocation.line()));

    fileName = fileName.substr(fileName.find_last_of('/') + 1);
    fileName = fileName.substr(0, fileName.find_last_of('.'));

    const size_t plen =
          (p1.length()               + aMessage.length()      + GpException_p2().length())
        + (p3.length()               + functioneName.length() + GpException_p2().length())
        + (GpException_p4().length() + fileName.length()      + GpException_p2().length())
        + (GpException_p5().length() + line.length());

    ToStrResT res;

    res.fullMessage.reserve(plen);

    res.fullMessage
        .append(p1).append(aMessage).append(GpException_p2())
        .append(p3).append(functioneName).append(GpException_p2())
        .append(GpException_p4()).append(fileName).append(GpException_p2())
        .append(GpException_p5()).append(line);

    res.message = std::string_view(res.fullMessage.data() + p1.length(), aMessage.size());

    return res;
}

}//namespace GPlatform
