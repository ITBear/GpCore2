#pragma once

#include <GpCore2/GpUtils/Types/Pointers/GpSharedPtrControlBlock.hpp>
#include <array>

namespace GPlatform {

template <typename  T>
class GpSharedPtrStorage final: public GpSharedPtrControlBlock
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSharedPtrStorage)

public:
    using this_type     = GpSharedPtrStorage<T>;
    using value_type    = T;

public:
    template<typename... Ts>    GpSharedPtrStorage  (Ts&&... aArgs);
    virtual                     ~GpSharedPtrStorage (void) noexcept override final = default;

    const T*                    ValuePtr            (void) const noexcept;
    T*                          ValuePtr            (void) noexcept;

private:
    alignas(T) std::array<std_byte_no_init, sizeof(T)>  iValueData;
};

template<typename T>
template<typename... Ts>
GpSharedPtrStorage<T>::GpSharedPtrStorage (Ts&&... aArgs):
GpSharedPtrControlBlock
{
    std::launder
    (
        GpMemOps::SEmplaceNew<T>
        (
            std::data(iValueData),
            std::forward<Ts>(aArgs)...
        )
    )
}
{
}

template<typename T>
const T*    GpSharedPtrStorage<T>::ValuePtr (void) const noexcept
{
    return std::launder(reinterpret_cast<const T*>(std::data(iValueData)));
}

template<typename T>
T*  GpSharedPtrStorage<T>::ValuePtr (void) noexcept
{
    return std::launder(reinterpret_cast<T*>(std::data(iValueData)));
}

}// namespace GPlatform

