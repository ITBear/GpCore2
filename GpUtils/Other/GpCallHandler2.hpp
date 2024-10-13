#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

#include <functional>
#include <optional>

namespace GPlatform {

class GpCallHandler2
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpCallHandler2)

public:
                    GpCallHandler2  (void) noexcept = default;
                    ~GpCallHandler2 (void) noexcept = default;

    inline void     Reset           (void) noexcept;

    template<typename FnT>
    void            CallIfFirst     (FnT aFn);

    template<typename FnT>
    void            CallIfNonFirst  (FnT aFn);

private:
    bool            iIsFirstCall = true;
};

void    GpCallHandler2::Reset (void) noexcept
{
    iIsFirstCall = true;
}

template<typename FnT>
void    GpCallHandler2::CallIfFirst (FnT aFn)
{
    if (iIsFirstCall)
    {
        aFn();
    }

    iIsFirstCall = false;
}

template<typename FnT>
void    GpCallHandler2::CallIfNonFirst (FnT aFn)
{
    if (!iIsFirstCall)
    {
        aFn();
    }

    iIsFirstCall = false;
}

}// namespace GPlatform
