#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/Config/IncludeExt/fmt.hpp>

#if defined(GP_USE_STRINGS)

#include <iostream>

namespace GPlatform {

GpOutUtils  GpOutUtils::sInstance;

GpOutUtils::GpOutUtils (void) noexcept
{
}

GpOutUtils::~GpOutUtils (void) noexcept
{
}

void    GpOutUtils::SetOutFns
(
    OutFnT aOutFn,
    OutFnT aErrFn
)
{
    iOutFn = std::move(aOutFn);
    iErrFn = std::move(aErrFn);
}

void    GpOutUtils::Out (std::string_view aStr) noexcept
{
    if (iOutFn)
    {
        try
        {
            iOutFn(aStr);
        } catch (const std::exception& e)
        {
            std::cerr << fmt::format
            (
                "[GpOutUtils::Out]: Exception caught while trying to print message: {}\nException: {}",
                aStr,
                e.what()
            );
            std::cerr.flush();
        } catch (...)
        {
            std::cerr << fmt::format
            (
                "[GpOutUtils::Out]: Exception caught while trying to print message: {}\nException: unknown",
                aStr
            );
            std::cerr.flush();
        }
    } else
    {
        std::cout << std::endl << aStr;
        std::cout.flush();
    }
}

void    GpOutUtils::Err (std::string_view aStr) noexcept
{
    if (iErrFn)
    {
        try
        {
            iErrFn(aStr);
        } catch (const std::exception& e)
        {
            std::cerr << fmt::format
            (
                "[GpOutUtils::Err]: Exception caught while trying to print message: {}\nException: {}",
                aStr,
                e.what()
            );
            std::cerr.flush();
        } catch (...)
        {
            std::cerr << fmt::format
            (
                "[GpOutUtils::Err]: Exception caught while trying to print message: {}\nException: unknown",
                aStr
            );
            std::cerr.flush();
        }
    } else
    {
        std::cerr << std::endl << aStr;
        std::cerr.flush();
    }
}

void    GpOutUtils::StdOut (std::string_view aStr) noexcept
{
    std::cout << std::endl << aStr;
    std::cout.flush();
}

void    GpOutUtils::StdErr (std::string_view aStr) noexcept
{
    std::cerr << std::endl << aStr;
    std::cerr.flush();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
