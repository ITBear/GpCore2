#include "GpException.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "GpExceptionsSink.hpp"

#if defined(GP_USE_EXCEPTIONS)

namespace GPlatform{

static constexpr std::string_view GpException_p1() {return ">_< Ouch! [GpException]: '"_sv;}
static constexpr std::string_view GpException_p2() {return "'\n"_sv;}
static constexpr std::string_view GpException_p3() {return "Function: '"_sv;}
static constexpr std::string_view GpException_p4() {return "    File: '"_sv;}
static constexpr std::string_view GpException_p5() {return "    Line: "_sv;}
static constexpr std::string_view GpException_p6() {return "\n"_sv;}

GpException::GpException (GpException&& aException) noexcept:
iWhat(std::move(aException.iWhat)),
iMsg(iWhat.data() + GpException_p1().length(), aException.iMsg.length()),
iSourceLocation(aException.iSourceLocation)
{
}

GpException::GpException
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept
try
{
    std::string_view    fileName(aSourceLocation.file_name());
    std::string_view    functioneName(aSourceLocation.function_name());
    std::string         line(std::to_string(aSourceLocation.line()));

    fileName = fileName.substr(fileName.find_last_of('/') + 1);
    fileName = fileName.substr(0, fileName.find_last_of('.'));

    constexpr size_t plen = GpException_p1().length()
                            + 3*GpException_p2().length()
                            + GpException_p3().length()
                            + GpException_p4().length()
                            + GpException_p5().length()
                            + GpException_p6().length();

    iWhat.reserve(plen + aMsg.length() + functioneName.length() + fileName.length() + 6);

    iWhat.append(GpException_p1())
         .append(aMsg).append(GpException_p2())
         .append(GpException_p3()).append(functioneName).append(GpException_p2())
         .append(GpException_p4()).append(fileName).append(GpException_p2())
         .append(GpException_p5()).append(line).append(GpException_p6());

    iMsg = std::string_view(iWhat.data() + GpException_p1().length(), aMsg.size());

    iSourceLocation = aSourceLocation;
} catch(...)
{
}

GpException::~GpException (void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
