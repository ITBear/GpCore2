#include <GpCore2/GpUtils/Other/ArgParser/GpArgParser.hpp>

namespace GPlatform {

GpArgParser::GpArgParser (void) noexcept
{
}

GpArgParser::~GpArgParser (void) noexcept
{
    GpArgParserArgumentBuilder::SP b = MakeSP<GpArgParserArgumentBuilder>();
    b.Clear();

    GpArgParserArgument::SP r = MakeSP<GpArgParserArgument>("123", false);
    r.Clear();

    iArgumentBuilder.Reset();
    iArguments.clear();
}

GpArgParserRes::SP  GpArgParser::Parse
(
    const size_t        aArgc,
    const char* const   aArgv[]
) const
{
    GpArgParserRes::ArgumentsVecT           resArguments;
    GpArgParserRes::PositionalArgumentsVecT resPositionalArguments;
    std::string                             resExecutablePathAndName;
    GpArgParserArgument*                    currentArgument = nullptr;

    for (size_t id = 0; id < aArgc; id++)
    {
        // Read argument name
        std::string_view    argVal{aArgv[id]};
        const size_t        argValSize          = std::size(argVal);
        bool                isArgNameOneChar    = false;

        if (id == 0) [[unlikely]]
        {
            resExecutablePathAndName = argVal;
            continue;
        }

        if ((argValSize >= 2) && (argVal.substr(0, 2) == "--"_sv)) // new argument with prefix: --
        {
            currentArgument     = nullptr;
            argVal              = argVal.substr(2);
        } else if ((argValSize >= 1) && (argVal.substr(0, 1) == "-"_sv))// new argument with prefix: -
        {
            currentArgument     = nullptr;
            argVal              = argVal.substr(1);
            isArgNameOneChar    = true;
        } else if (currentArgument != nullptr)
        {
            currentArgument->AddValue(argVal, ' ');
            currentArgument     = nullptr;
            continue;
        } else if (currentArgument == nullptr)
        {
            // Positional argument
            resPositionalArguments.emplace_back(argVal);
            continue;
        }

        std::string_view    argValName          = argVal;
        std::string_view    argValValue;
        char                argValSeparator     = 0;
        bool                storeArgValValue    = false;

        if (   (isArgNameOneChar == true)
            && (std::size(argVal) > 1))
        {
            if (argVal[1] == '=')
            {
                argValName      = argVal.substr(0, 1);
                argValValue     = argVal.substr(2);
                argValSeparator = '=';
            } else
            {
                argValName      = argVal.substr(0, 1);
                argValValue     = argVal.substr(1);
                argValSeparator = 0;
            }

            storeArgValValue    = true;
        } else if (auto pos = argValName.find('='); pos != std::string::npos)
        {
            argValName          = argVal.substr(0, pos);
            argValValue         = argVal.substr(pos + 1);
            storeArgValValue    = true;
            argValSeparator     = '=';
        }

        // Try to find argument by name in resArguments
        auto iter = std::find_if
        (
            std::begin(resArguments),
            std::end(resArguments),
            [argValName](const auto& aArgument)
            {
                return aArgument.V().Names().count(argValName) > 0;
            }
        );

        if (iter != std::end(resArguments))
        {
            currentArgument = iter->P();
        } else
        {
            // Try to find argument by name in iArguments
            if (auto argIter = iArguments.find(argValName); argIter != std::end(iArguments))
            {
                // Add argument to resArguments (known)
                auto newElement = resArguments.emplace_back(MakeSP<GpArgParserArgument>(argIter->second.V()));
                currentArgument = newElement.P();
            } else
            {
                VERIFY
                (
                    iIsEnableUnknownArguments,
                    [argValName]()
                    {
                        return fmt::format
                        (
                            "Unknown argument name '{}'",
                            argValName
                        );
                    }
                );

                // Add argument to resArguments (unknown)
                auto newElement = resArguments.emplace_back(MakeSP<GpArgParserArgument>(std::string{argValName}, true));
                currentArgument = newElement.P();
            }
        }

        if (storeArgValValue)
        {
            currentArgument->AddValue(argValValue, argValSeparator);
            currentArgument = nullptr;
        }
    }

    return MakeSP<GpArgParserRes>
    (
        std::move(resArguments),
        std::move(resPositionalArguments),
        std::move(resExecutablePathAndName)
    );
}

GpArgParserArgumentBuilder& GpArgParser::NextArgument (void)
{
    iArgumentBuilder.SetArgParser(*this);
    iArgumentBuilder.Reset();

    return iArgumentBuilder;
}

GpArgParser&    GpArgParser::AddArgument (GpArgParserArgument::SP aArgumentSP)
{
    const GpArgParserArgument& argument = aArgumentSP.V();

    // Check argument names
    VERIFY
    (
        !argument.Names().empty(),
        "No name was set for the argument"
    );

    // Add argument by names
    for (std::string_view name: argument.Names())
    {
        // Check if name is not empty
        VERIFY
        (
            !name.empty(),
            "Argument name is empty"
        );

        // Check if name contains only allowed characters
        static const boost::container::small_vector<std::tuple<char, char>, 8> sAllowedCharRanges =
        {
            {'a', 'z'},
            {'A', 'Z'},
            {'0', '9'},
            {'_', '_'},
            {'-', '-'}
        };

        VERIFY
        (
            StrOps::SContainsOnlyRanges(name, sAllowedCharRanges),
            [name]()
            {
                return fmt::format
                (
                    "Argument name '{}' contains disallowed characters",
                    name
                );
            }
        );

        // Check if name is unique
        VERIFY
        (
            iArguments.count(name) == 0,
            [name]()
            {
                return fmt::format
                (
                    "Argument name '{}' is not unique",
                    name
                );
            }
        );

        iArguments.emplace(std::string{name}, std::move(aArgumentSP));
    }

    return *this;
}

void    GpArgParser::EnableUnknownArguments (void) noexcept
{
    iIsEnableUnknownArguments = true;
}

void    GpArgParser::DisableUnknownArguments (void) noexcept
{
    iIsEnableUnknownArguments = false;
}

}// namespace GPlatform
