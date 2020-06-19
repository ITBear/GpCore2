#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpRawPtrR.hpp"
#include "../Pointers/GpRawPtrRW.hpp"
#include "GpBytesArray.hpp"

namespace GPlatform {

class GpRawPtrByteR: public GpRawPtrR<const std::byte*>
{
public:
    using this_type = GpRawPtrByteR;

public:
                        GpRawPtrByteR       (void) = delete;
    constexpr           GpRawPtrByteR       (const this_type& aRawPtr) noexcept: GpRawPtrR(aRawPtr) {}
    constexpr           GpRawPtrByteR       (this_type&& aRawPtr) noexcept: GpRawPtrR(std::move(aRawPtr)) {}
    constexpr           GpRawPtrByteR       (const std::byte* aBytes, const size_byte_t aSize): GpRawPtrR(aBytes, count_t::SMake(aSize.Value())) {}
                        GpRawPtrByteR       (std::string_view aStr): GpRawPtrR(reinterpret_cast<const std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                        GpRawPtrByteR       (const std::string& aStr): GpRawPtrR(reinterpret_cast<const std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                        GpRawPtrByteR       (const GpBytesArray& aBytes): GpRawPtrR(aBytes.data(), count_t::SMake(aBytes.size())) {}
};

class GpRawPtrByteRW: public GpRawPtrRW<std::byte*>
{
public:
    using this_type = GpRawPtrByteRW;

public:
                        GpRawPtrByteRW      (void) = delete;
    constexpr           GpRawPtrByteRW      (const this_type& aRawPtr) noexcept: GpRawPtrRW(aRawPtr) {}
    constexpr           GpRawPtrByteRW      (this_type&& aRawPtr) noexcept: GpRawPtrRW(std::move(aRawPtr)) {}
    constexpr           GpRawPtrByteRW      (std::byte* aBytes, const size_byte_t aSize): GpRawPtrRW(aBytes, count_t::SMake(aSize.Value())) {}
                        GpRawPtrByteRW      (std::string& aStr): GpRawPtrRW(reinterpret_cast<std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                        GpRawPtrByteRW      (GpBytesArray& aBytes): GpRawPtrRW(aBytes.data(), count_t::SMake(aBytes.size())) {}
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
