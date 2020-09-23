#pragma once

#include "../../Config/GpConfig.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

#include <functional>
#include <stack>

namespace GPlatform {

template<typename FnT>
class GpOnThrowStackUnwindFn
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpOnThrowStackUnwindFn)

public:
                    GpOnThrowStackUnwindFn  (void) noexcept {}
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

}//GPlatform
