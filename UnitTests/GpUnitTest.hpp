#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include "GpUnitTestCtx.hpp"
#include "../Types/Strings/GpStringOps.hpp"
#include <functional>

namespace GPlatform {

class GPCORE_API GpUnitTest
{
    CLASS_REMOVE_CTRS(GpUnitTest)

public:
    //https://github.com/google/googletest/blob/master/googletest/docs/primer.md

    static void TEST (std::string_view      aTestName,
                      GpUnitTestCtx::SP     aCtx,
                      std::function<void()> aFn);

    template<typename T>
    static void ASSERT_EQ (const T&                 aArg1,
                           const T&                 aArg2,
                           std::string_view         aUserMessageOnError = ""_sv,
                           const SourceLocationT&   aLocation           = SourceLocationT::current())
    {
        if (aArg1 == aArg2)
        {
            return;
        }

        THROW_COMPARE_ERROR<T>(aArg1, aArg2, "EQ"_sv, "must be EQUAL to"_sv, aUserMessageOnError, aLocation);
    }

    template<typename T>
    static void EXPECT_EQ (const T&                 aArg1,
                           const T&                 aArg2,
                           std::string_view         aUserMessageOnError = ""_sv,
                           const SourceLocationT&   aLocation           = SourceLocationT::current())
    {
        if (aArg1 == aArg2)
        {
            return;
        }

        THROW_COMPARE_ERROR<T>(aArg1, aArg2, "EQ"_sv, "must be EQUAL to"_sv, aUserMessageOnError, aLocation);
    }

    template<typename T>
    static void ASSERT_NOT_EQ (const T&             aArg1,
                               const T&             aArg2,
                               std::string_view     aUserMessageOnError = ""_sv,
                               const SourceLocationT&   aLocation       = SourceLocationT::current())
    {
        if (aArg1 != aArg2)
        {
            return;
        }

        THROW_COMPARE_ERROR<T>(aArg1, aArg2, "NOT_EQ"_sv, "must be NOT EQUAL to"_sv, aUserMessageOnError, aLocation);
    }

private:
    template<typename T>
    static void THROW_COMPARE_ERROR (const T&               aArg1,
                                     const T&               aArg2,
                                     std::string_view       aTestName,
                                     std::string_view       aCompareText,
                                     std::string_view       aUserMessage,
                                     const SourceLocationT& aLocation)
    {
        std::string message;
        message.reserve(512);

        message
            .append("FAIL [").append(aTestName).append("]:"_sv)
            .append(" '"_sv).append(StrOps::SToString<T>(aArg1)).append("' "_sv)
            .append(aCompareText)
            .append(" '"_sv).append(StrOps::SToString<T>(aArg2)).append("'"_sv);

        if (aUserMessage.length() > 0)
        {
            message.append(", "_sv).append(aUserMessage);
        } else
        {
            message.append("."_sv);
        }

        throw ::GPlatform::GpUnitTestException(message, aLocation);
    }
};

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
