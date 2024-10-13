#pragma once

#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgumentBuilder.hpp>
#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserRes.hpp>

namespace GPlatform {

class GP_UTILS_API GpArgParser
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpArgParser)
    CLASS_DD(GpArgParser)

    using ArgumentMapT = boost::container::small_flat_map<std::string, GpArgParserArgument::SP, 64, std::less<>>;

public:
                                    GpArgParser             (void) noexcept;
                                    ~GpArgParser            (void) noexcept;

    GpArgParserRes::SP              Parse                   (size_t             aArgc,
                                                             const char* const  aArgv[]) const;
    GpArgParserArgumentBuilder&     NextArgument            (void);
    GpArgParser&                    AddArgument             (GpArgParserArgument::SP aArgument);
    void                            EnableUnknownArguments  (void) noexcept;
    void                            DisableUnknownArguments (void) noexcept;

private:
    GpArgParserArgumentBuilder      iArgumentBuilder;
    ArgumentMapT                    iArguments;
    bool                            iIsEnableUnknownArguments   = false;
};

}// namespace GPlatform
