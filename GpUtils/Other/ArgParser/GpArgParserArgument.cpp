#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgument.hpp>

namespace GPlatform {

GpArgParserArgument::GpArgParserArgument
(
    StringSetT&&    aNames,
    std::string     aDescription,
    std::string     aDefaultValue
):
iNames       {std::move(aNames)},
iDescription {std::move(aDescription)},
iDefaultValue{std::move(aDefaultValue)}
{
}

GpArgParserArgument::GpArgParserArgument
(
    std::string aName,
    const bool  aIsUnknownArgument
):
iNames            {std::move(aName)},
iIsUnknownArgument{aIsUnknownArgument}
{
}

GpArgParserArgument::GpArgParserArgument (const GpArgParserArgument& aArgument):
iNames            {aArgument.iNames},
iDescription      {aArgument.iDescription},
iDefaultValue     {aArgument.iDefaultValue},
iValues           {aArgument.iValues},
iIsUnknownArgument{aArgument.iIsUnknownArgument}
{
}

GpArgParserArgument::GpArgParserArgument (GpArgParserArgument&& aArgument):
iNames            {std::move(aArgument.iNames)},
iDescription      {std::move(aArgument.iDescription)},
iDefaultValue     {std::move(aArgument.iDefaultValue)},
iValues           {std::move(aArgument.iValues)},
iIsUnknownArgument{std::move(aArgument.iIsUnknownArgument)}
{
}

GpArgParserArgument::~GpArgParserArgument (void) noexcept
{
}

void    GpArgParserArgument::AddValue
(
    std::string_view    aValue,
    const char          aSeparator
)
{
    iValues.emplace_back(aValue, aSeparator);
}

}// namespace GPlatform
