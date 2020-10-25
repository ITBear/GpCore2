#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpException.hpp"
#include <functional>

namespace GPlatform{

class GPCORE_API GpExceptionUtils
{
public:
    template<typename ExpectexExT>
    static void         SExpectThrow            (std::function<void()>                          aThrowableFn,
                                                 std::function<bool(const ExpectexExT& aEx)>    aCatchFn,
                                                 std::function<void()>                          aThrowFn);
};

template<typename ExpectexExT>
void    GpExceptionUtils::SExpectThrow (std::function<void()>                       aThrowableFn,
                                        std::function<bool(const ExpectexExT& aEx)> aCatchFn,
                                        std::function<void()>                       aThrowFn)
{
    bool isExCatched = false;

    try
    {
        aThrowableFn();
    } catch (const ExpectexExT& aEx)
    {
        if (aCatchFn(aEx))
        {
            isExCatched = true;
        }
    }

    if (!isExCatched)
    {
        aThrowFn();
    }
}

}//namespace GPlatform

#endif // GPEXCEPTIONUTILS_HPP
