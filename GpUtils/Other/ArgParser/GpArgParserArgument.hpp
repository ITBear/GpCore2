#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/GpUtils_global.hpp>

#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>

namespace GPlatform {

class GP_UTILS_API GpArgParserArgument
{
public:
    CLASS_REMOVE_CTRS_DEFAULT(GpArgParserArgument)
    CLASS_DD(GpArgParserArgument)

    using StringSetT    = boost::container::small_flat_set<std::string, 2, std::less<>>;
    using StringVecT    = boost::container::small_vector<std::tuple<std::string/*value*/, char/*separator*/>, 8>;

public:
                        GpArgParserArgument     (StringSetT&&   aNames,
                                                 std::string    aDescription,
                                                 std::string    aDefaultValue);
                        GpArgParserArgument     (std::string    aName,
                                                 bool           aIsUnknownArgument);
                        GpArgParserArgument     (const GpArgParserArgument& aArgument);
                        GpArgParserArgument     (GpArgParserArgument&& aArgument);
                        ~GpArgParserArgument    (void) noexcept;

    const StringSetT&   Names                   (void) const noexcept {return iNames;}
    std::string_view    Description             (void) const noexcept {return iDescription;}
    std::string_view    DefaultValue            (void) const noexcept {return iDefaultValue;}
    const StringVecT&   Values                  (void) const noexcept {return iValues;}
    StringVecT&         Values                  (void) noexcept {return iValues;}
    bool                IsUnknownArgument       (void) const noexcept {return iIsUnknownArgument;}

    void                AddValue                (std::string_view   aValue,
                                                 char               aSeparator);

private:
    StringSetT          iNames;
    std::string         iDescription;
    std::string         iDefaultValue;
    StringVecT          iValues;
    bool                iIsUnknownArgument = false;
};

}// namespace GPlatform
