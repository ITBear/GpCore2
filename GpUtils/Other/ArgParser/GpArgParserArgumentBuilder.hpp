#pragma once

#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgument.hpp>

namespace GPlatform {

class GpArgParser;

class GP_UTILS_API GpArgParserArgumentBuilder
{
public:
    CLASS_REMOVE_CTRS_DEFAULT(GpArgParserArgumentBuilder)
    CLASS_DD(GpArgParserArgumentBuilder)

    using StringSetT = GpArgParserArgument::StringSetT;

public:
                                    GpArgParserArgumentBuilder  (GpArgParser& aArgParser) noexcept;
                                    ~GpArgParserArgumentBuilder (void) noexcept;

    void                            Reset                       (void);
    GpArgParser&                    Done                        (void);

    GpArgParserArgumentBuilder&     AddName                     (std::string_view aName);
    GpArgParserArgumentBuilder&     Description                 (std::string_view aDescription);
    GpArgParserArgumentBuilder&     DefaultValue                (std::string_view aValue);

private:
    GpArgParser&                    iArgParser;
    StringSetT                      iNames;
    std::string                     iDescription;
    std::string                     iDefaultValue;
};

}// namespace GPlatform
