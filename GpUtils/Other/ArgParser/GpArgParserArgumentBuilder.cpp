#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserArgumentBuilder.hpp>
#include <GpCore2/GpUtils/Other/ArgParser/GpArgParser.hpp>

namespace GPlatform {

GpArgParserArgumentBuilder::GpArgParserArgumentBuilder (void) noexcept
{
}

GpArgParserArgumentBuilder::~GpArgParserArgumentBuilder (void) noexcept
{
}

void    GpArgParserArgumentBuilder::SetArgParser (GpArgParser& aArgParser)
{
    iArgParser = &aArgParser;
}

void    GpArgParserArgumentBuilder::Reset (void)
{
    iNames.clear();
    iDescription.clear();
    iDefaultValue.clear();
}

void    GpArgParserArgumentBuilder::Done (void)
{
    iArgParser->AddArgument
    (
        MakeSP<GpArgParserArgument>
        (
            std::move(iNames),
            std::move(iDescription),
            std::move(iDefaultValue)
        )
    );

    Reset();
}

GpArgParserArgumentBuilder& GpArgParserArgumentBuilder::AddName (std::string_view aName)
{
    VERIFY
    (
        iNames.count(aName) == 0,
        [aName]()
        {
            return fmt::format
            (
                "Argument name '{}' is not unique",
                aName
            );
        }
    );

    iNames.insert(std::string{aName});

    return *this;
}

GpArgParserArgumentBuilder& GpArgParserArgumentBuilder::Description (std::string_view aDescription)
{
    iDescription = aDescription;

    return *this;
}

GpArgParserArgumentBuilder& GpArgParserArgumentBuilder::DefaultValue (std::string_view aValue)
{
    iDefaultValue = aValue;

    return *this;
}

}// namespace GPlatform
