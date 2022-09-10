#pragma once

#include "../../GpUtils_global.hpp"

namespace GPlatform {

class GpBool
{
public:
    enum VAL
    {
        FALSE,
        TRUE
    };

public:
                    GpBool      (void) noexcept {}
                    GpBool      (const GpBool& aBool) noexcept: iValue(aBool.iValue) {}
                    GpBool      (const VAL aValue) noexcept: iValue(aValue == TRUE) {}

    GpBool&         operator=   (const GpBool& aBool) noexcept {iValue = aBool.iValue; return *this;}
    GpBool&         operator=   (const VAL aValue) noexcept {iValue = (aValue == TRUE); return *this;}

    bool            Value       (void) const noexcept {return iValue;}

private:
    bool            iValue  = false;
};

}//namespace GPlatform
