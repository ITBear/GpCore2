#pragma once

#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgument.hpp>

namespace GPlatform {

class GpArgParser;

class GP_UTILS_API GpArgParserArgumentBuilder
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpArgParserArgumentBuilder)
    CLASS_DD(GpArgParserArgumentBuilder)

    using StringSetT = GpArgParserArgument::StringSetT;

public:
                                GpArgParserArgumentBuilder  (void) noexcept;
                                ~GpArgParserArgumentBuilder (void) noexcept;

    void                        SetArgParser                (GpArgParser& aArgParser);
    void                        Reset                       (void);
    void                        Done                        (void);

    GpArgParserArgumentBuilder& AddName                     (std::string_view aName);
    GpArgParserArgumentBuilder& Description                 (std::string_view aDescription);
    GpArgParserArgumentBuilder& DefaultValue                (std::string_view aValue);

private:
    GpArgParser*    iArgParser = nullptr;
    StringSetT      iNames;
    std::string     iDescription;
    std::string     iDefaultValue;
};

}// namespace GPlatform
