#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

template <typename T>
class GpHysteresis
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHysteresis)

    using value_t   = T;
    using this_type = GpHysteresis<T>;

public:
                GpHysteresis    (const T    aUpLevel,
                                 const T    aDownLevel) noexcept;
                GpHysteresis    (const T    aUpLevel,
                                 const T    aDownLevel,
                                 const T    aValue) noexcept;
                ~GpHysteresis   (void) noexcept;

    bool        Update          (const T    aValue) noexcept;
    bool        SwitchState     (void) const noexcept;


private:
    value_t     iUpLevel        = T();
    value_t     iDownLevel      = T();
    value_t     iValue          = T();
    bool        iSwitchState    = false;
};

template <typename T>
GpHysteresis<T>::GpHysteresis
(
    const T aUpLevel,
    const T aDownLevel
) noexcept:
iUpLevel(aUpLevel),
iDownLevel(aDownLevel)
{
}

template <typename T>
GpHysteresis<T>::GpHysteresis
(
    const T aUpLevel,
    const T aDownLevel,
    const T aValue
) noexcept:
iUpLevel(aUpLevel),
iDownLevel(aDownLevel),
iValue(aValue)
{
}

template <typename T>
bool    GpHysteresis<T>::Update (const T aValue) noexcept
{
    iValue = aValue;

    if (iSwitchState == true)
    {
        if (iValue < iDownLevel)
        {
            iSwitchState = false;
        }
    } else
    {
        if (iValue > iUpLevel)
        {
            iSwitchState = true;
        }
    }

    return iSwitchState;
}

template <typename T>
bool    GpHysteresis<T>::SwitchState (void) const noexcept
{
    return iSwitchState;
}

}//namespace GPlatform
