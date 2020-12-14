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

    template<typename ExpectexExT>
    static void         SCatch                  (std::function<void()>                          aThrowableFn,
                                                 std::function<void(const ExpectexExT& aEx)>    aCatchFn);
};

template<typename ExpectexExT>
void    GpExceptionUtils::SExpectThrow (std::function<void()>                       aThrowableFn,
                                        std::function<bool(const ExpectexExT& aEx)> aCatchFn,
                                        std::function<void()>                       aThrowFn)
{
    bool isCatch = false;

    try
    {
        aThrowableFn();
    } catch (const ExpectexExT& aEx/*expect this exception*/)
    {
        isCatch = aCatchFn(aEx);
    }

    if (!isCatch)
    {
        aThrowFn();
    }
}

template<typename ExpectexExT>
void    GpExceptionUtils::SCatch (std::function<void()>                         aThrowableFn,
                                  std::function<void(const ExpectexExT& aEx)>   aCatchFn)
{
    try
    {
        aThrowableFn();
    } catch (const ExpectexExT& aEx/*catch exception*/)
    {
        aCatchFn(aEx);
    }
}

}//namespace GPlatform

#endif // GPEXCEPTIONUTILS_HPP
