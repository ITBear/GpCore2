#include <GpCore2/GpUtils/Other/ArgParser/GpArgParserRes.hpp>

namespace GPlatform {

GpArgParserRes::GpArgParserRes (void) noexcept
{
}

GpArgParserRes::GpArgParserRes (const GpArgParserRes& aArgParserRes):
iArguments            {aArgParserRes.iArguments},
iPositionalArguments  {aArgParserRes.iPositionalArguments},
iExecutablePathAndName{aArgParserRes.iExecutablePathAndName}
{
}

GpArgParserRes::GpArgParserRes (GpArgParserRes&& aArgParserRes) noexcept:
iArguments            {std::move(aArgParserRes.iArguments)},
iPositionalArguments  {std::move(aArgParserRes.iPositionalArguments)},
iExecutablePathAndName{std::move(aArgParserRes.iExecutablePathAndName)}
{
}

GpArgParserRes::GpArgParserRes
(
    ArgumentsVecT&&             aArguments,
    PositionalArgumentsVecT&&   aPositionalArguments,
    std::string&&               aExecutablePathAndName
) noexcept:
iArguments            {std::move(aArguments)},
iPositionalArguments  {std::move(aPositionalArguments)},
iExecutablePathAndName{std::move(aExecutablePathAndName)}
{
}

GpArgParserRes::~GpArgParserRes (void) noexcept
{
}

GpArgParserArgument::C::Opt::CRef   GpArgParserRes::FindArgument (std::string_view aName) const noexcept
{
    for (const GpArgParserArgument::SP& argSP: iArguments)
    {
        if (argSP->Names().count(aName) > 0)
        {
            return argSP.V();
        }
    }

    return std::nullopt;
}

std::string GpArgParserRes::ToString
(
    const bool aUseKnownArguments,
    const bool aUseUnknownArguments,
    const bool aUsePositionalArguments
) const
{
    std::string resStr;
    resStr.reserve(1024);

    if (aUseKnownArguments || aUseUnknownArguments)
    {
        for (const GpArgParserArgument::SP& argSP: iArguments)
        {
            const GpArgParserArgument& arg = argSP.V();

            if (arg.IsUnknownArgument())
            {
                if (!aUseUnknownArguments)
                {
                    continue;
                }
            } else
            {
                if (!aUseKnownArguments)
                {
                    continue;
                }
            }

            std::string_view    argName     = *arg.Names().begin();
            const auto&         argValues   = arg.Values();

            if (!resStr.empty())
            {
                resStr.push_back(' ');
            }

            if (argValues.empty())
            {
                if (std::size(argName) == 1)
                {
                    resStr.append("-");
                } else
                {
                    resStr.append("--");
                }

                resStr.append(argName);
            } else
            {
                bool isFirstTime = true;

                for (const auto& [val, separator]: argValues)
                {
                    if (!isFirstTime)
                    {
                        resStr.push_back(' ');
                    } else
                    {
                        isFirstTime = false;
                    }

                    const std::string escapedVal = StrOps::SEscapeForSyscall(val);

                    if (std::size(argName) == 1)
                    {
                        resStr.append("-");
                    } else
                    {
                        resStr.append("--");
                    }

                    resStr.append(argName);

                    if (separator != 0)
                    {
                        resStr.push_back(separator);
                    }

                    if (escapedVal == val)
                    {
                        resStr.append(val);
                    } else
                    {
                        resStr.append("\"");
                        resStr.append(escapedVal);
                        resStr.append("\"");
                    }
                }
            }
        }
    }

    if (aUsePositionalArguments)
    {
        for (std::string_view arg: iPositionalArguments)
        {
            if (!resStr.empty())
            {
                resStr.push_back(' ');
            }

            resStr.append(arg);
        }
    }

    return resStr;
}

}// namespace GPlatform
