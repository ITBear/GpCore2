#pragma once

#include "../GpMacro.hpp"

#include <functional>

namespace GPlatform {

class GpCallOnce
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpCallOnce)

public:
                    GpCallOnce  (void) noexcept = default;
                    ~GpCallOnce (void) noexcept = default;

    template<typename FnT>
    void            Call        (FnT aFn)
    {
        if (!iIsFirstCall)
        {
            return;
        }

        iIsFirstCall = false;
        aFn();
    }

private:
    bool            iIsFirstCall = true;
};

}//GPlatform
