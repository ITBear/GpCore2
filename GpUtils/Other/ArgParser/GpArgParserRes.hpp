#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgument.hpp>

namespace GPlatform {

class GP_UTILS_API GpArgParserRes
{
public:
    CLASS_DD(GpArgParserRes)

    using ArgumentsVecT             = std::vector<GpArgParserArgument::SP>;
    using PositionalArgumentsVecT   = std::vector<std::string>;

public:
                                        GpArgParserRes          (void) noexcept;
                                        GpArgParserRes          (const GpArgParserRes& aArgParserRes);
                                        GpArgParserRes          (GpArgParserRes&& aArgParserRes) noexcept;
                                        GpArgParserRes          (ArgumentsVecT&&            aArguments,
                                                                 PositionalArgumentsVecT&&  aPositionalArguments,
                                                                 std::string&&              aExecutablePathAndName) noexcept;
                                        ~GpArgParserRes         (void) noexcept;

    const ArgumentsVecT&                Arguments               (void) const noexcept {return iArguments;}
    ArgumentsVecT&                      Arguments               (void) noexcept {return iArguments;}
    const PositionalArgumentsVecT&      PositionalArguments     (void) const noexcept {return iPositionalArguments;}
    PositionalArgumentsVecT&            PositionalArguments     (void) noexcept {return iPositionalArguments;}
    std::string_view                    ExecutablePathAndName   (void) const noexcept {return iExecutablePathAndName;}

    GpArgParserArgument::C::Opt::CRef   FindArgument            (std::string_view aName) const noexcept;

    std::string                         ToString                (bool aUseKnownArguments,
                                                                 bool aUseUnknownArguments,
                                                                 bool aUsePositionalArguments) const;

private:
    ArgumentsVecT                       iArguments;
    PositionalArgumentsVecT             iPositionalArguments;
    std::string                         iExecutablePathAndName;
};

}// namespace GPlatform
