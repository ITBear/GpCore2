#pragma once

#include "../Macro/GpMacroClass.hpp"

namespace GPlatform::Algo {

template <typename T>
class GpHysteresis
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHysteresis)

    using value_t   = T;
    using this_type = GpHysteresis<T>;

public:
                GpHysteresis    (const T    aDownLevel,
                                 const T    aUpLevel) noexcept;
                GpHysteresis    (const T    aDownLevel,
                                 const T    aUpLevel,
                                 const T    aValue) noexcept;
                ~GpHysteresis   (void) noexcept {}

    bool        Update          (const T    aValue) noexcept;
    bool        SwitchState     (void) const noexcept;

private:
    value_t     iDownLevel      = T();
    value_t     iUpLevel        = T();
    value_t     iValue          = T();
    bool        iSwitchState    = false;
};

template <typename T>
GpHysteresis<T>::GpHysteresis
(
    const T aDownLevel,
    const T aUpLevel
) noexcept:
iDownLevel(aDownLevel),
iUpLevel(aUpLevel)
{
}

template <typename T>
GpHysteresis<T>::GpHysteresis
(
    const T aDownLevel,
    const T aUpLevel,
    const T aValue
) noexcept:
iDownLevel(aDownLevel),
iUpLevel(aUpLevel),
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

}//namespace GPlatform::Algo
