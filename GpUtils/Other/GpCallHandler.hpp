#pragma once

#include "../Macro/GpMacroClass.hpp"

#include <functional>
#include <optional>

namespace GPlatform {

template <typename FnFirstCallT,
          typename FnOtherCallsT>
class GpCallHandler
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpCallHandler)

public:
                    GpCallHandler   (std::optional<FnFirstCallT>    aOnFirstCallFn,
                                     std::optional<FnOtherCallsT>   aOnOtherCallsFn) noexcept;
                    ~GpCallHandler  (void) noexcept = default;

    void            Reset           (void) noexcept;
    void            Call            (void);

private:
    std::optional<FnFirstCallT>     iOnFirstCallFn;
    std::optional<FnOtherCallsT>    iOnOtherCallsFn;
    bool                            iIsFirstCall = true;
};

template <typename FnFirstCallT,
          typename FnOtherCallsT>
GpCallHandler<FnFirstCallT, FnOtherCallsT>::GpCallHandler
(
    std::optional<FnFirstCallT>     aOnFirstCallFn,
    std::optional<FnOtherCallsT>    aOnOtherCallsFn
) noexcept:
iOnFirstCallFn(aOnFirstCallFn),
iOnOtherCallsFn(aOnOtherCallsFn)
{
}

template <typename FnFirstCallT,
          typename FnOtherCallsT>
void    GpCallHandler<FnFirstCallT, FnOtherCallsT>::Reset (void) noexcept
{
    iIsFirstCall = true;
}

template <typename FnFirstCallT,
          typename FnOtherCallsT>
void    GpCallHandler<FnFirstCallT, FnOtherCallsT>::Call (void)
{
    if (!iIsFirstCall)
    {
        if (iOnOtherCallsFn.has_value())
        {
            iOnOtherCallsFn.value()();
        }
    } else//First call
    {
        iIsFirstCall = false;
        if (iOnFirstCallFn.has_value())
        {
            iOnFirstCallFn.value()();
        }
    }
}

}//GPlatform
