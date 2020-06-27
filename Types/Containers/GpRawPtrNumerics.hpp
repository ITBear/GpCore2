#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpRawPtrR.hpp"
#include "../Pointers/GpRawPtrRW.hpp"

namespace GPlatform {

template<typename T>
class GpRawPtrNumericR: public GpRawPtrR<T>
{
public:
    using this_type = GpRawPtrNumericR<T>;

public:
                        GpRawPtrNumericR    (void) = delete;
    constexpr           GpRawPtrNumericR    (const this_type& aRawPtr) noexcept: GpRawPtrR<T>::GpRawPtrR(aRawPtr) {}
    constexpr           GpRawPtrNumericR    (this_type&& aRawPtr) noexcept: GpRawPtrR<T>::GpRawPtrR(std::move(aRawPtr)) {}
    constexpr           GpRawPtrNumericR    (T aData, const count_t aCount): GpRawPtrR<T>::GpRawPtrR(aData, aCount) {}

    template<typename ContainerT>
    constexpr           GpRawPtrNumericR    (const ContainerT& aContainer): GpRawPtrR<T>::GpRawPtrR(aContainer.data(), count_t::SMake(aContainer.size())) {}
};

template<typename T>
class GpRawPtrNumericRW: public GpRawPtrRW<T>
{
public:
    using this_type = GpRawPtrNumericRW<T>;

public:
                        GpRawPtrNumericRW       (void) = delete;
    constexpr           GpRawPtrNumericRW       (this_type& aRawPtr) noexcept: GpRawPtrRW<T>::GpRawPtrRW(aRawPtr) {}
    constexpr           GpRawPtrNumericRW       (this_type&& aRawPtr) noexcept: GpRawPtrRW<T>::GpRawPtrRW(std::move(aRawPtr)) {}
    constexpr           GpRawPtrNumericRW       (T aData, const count_t aCount): GpRawPtrRW<T>::GpRawPtrRW(aData, aCount) {}

    template<typename ContainerT>
    constexpr           GpRawPtrNumericRW       (ContainerT& aContainer): GpRawPtrRW<T>::GpRawPtrRW(aContainer.data(), count_t::SMake(aContainer.size())) {}
};

using GpRawPtrUI8_R     = GpRawPtrNumericR<const u_int_8*>;
using GpRawPtrUI8_RW    = GpRawPtrNumericRW<u_int_8*>;
using GpRawPtrSI8_R     = GpRawPtrNumericR<const s_int_8*>;
using GpRawPtrSI8_RW    = GpRawPtrNumericRW<s_int_8*>;
using GpRawPtrUI16_R    = GpRawPtrNumericR<const u_int_16*>;
using GpRawPtrUI16_RW   = GpRawPtrNumericRW<u_int_16*>;
using GpRawPtrSI16_R    = GpRawPtrNumericR<const s_int_16*>;
using GpRawPtrSI16_RW   = GpRawPtrNumericRW<s_int_16*>;
using GpRawPtrUI32_R    = GpRawPtrNumericR<const u_int_32*>;
using GpRawPtrUI32_RW   = GpRawPtrNumericRW<u_int_32*>;
using GpRawPtrSI32_R    = GpRawPtrNumericR<const s_int_32*>;
using GpRawPtrSI32_RW   = GpRawPtrNumericRW<s_int_32*>;
using GpRawPtrUI64_R    = GpRawPtrNumericR<const u_int_64*>;
using GpRawPtrUI64_RW   = GpRawPtrNumericRW<u_int_64*>;
using GpRawPtrSI64_R    = GpRawPtrNumericR<const s_int_64*>;
using GpRawPtrSI64_RW   = GpRawPtrNumericRW<s_int_64*>;

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
