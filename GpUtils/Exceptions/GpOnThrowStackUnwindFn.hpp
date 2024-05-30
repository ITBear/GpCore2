#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include "../Macro/GpMacroClass.hpp"

#include <functional>
#include <stack>
#include <exception>

namespace GPlatform {

template<typename FnT>
class GpOnThrowStackUnwindFn
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpOnThrowStackUnwindFn)

public:
                    GpOnThrowStackUnwindFn  (void) noexcept = default;
                    GpOnThrowStackUnwindFn  (FnT aFn) {Push(aFn);}
                    ~GpOnThrowStackUnwindFn (void) noexcept {Unwind();}

    void            Push                    (FnT aFn)
    {
        iFnStack.push(aFn);
    }

    void            Unwind                  (void) noexcept
    {
        if (std::uncaught_exceptions() == 0)
        {
            return;
        }

        while (!iFnStack.empty())
        {
            iFnStack.top()();
            iFnStack.pop();
        }
    }

private:
    std::stack<FnT> iFnStack;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
