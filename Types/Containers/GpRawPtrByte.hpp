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
    constexpr               GpRawPtrByteR       (const this_type& aRawPtr) noexcept: GpRawPtrR(aRawPtr) {}
    constexpr               GpRawPtrByteR       (this_type&& aRawPtr) noexcept: GpRawPtrR(std::move(aRawPtr)) {}
    constexpr               GpRawPtrByteR       (const std::byte* aBytes, const size_byte_t aSize): GpRawPtrR(aBytes, aSize) {}
    constexpr               GpRawPtrByteR       (const std::byte* aBytes, const count_t aCount): GpRawPtrR(aBytes, aCount) {}
                            GpRawPtrByteR       (std::string_view aStr): GpRawPtrR(reinterpret_cast<const std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                            GpRawPtrByteR       (const std::string& aStr): GpRawPtrR(reinterpret_cast<const std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                            GpRawPtrByteR       (const GpBytesArray& aBytes): GpRawPtrR(aBytes.data(), count_t::SMake(aBytes.size())) {}

    template<typename ContainerT>
    constexpr               GpRawPtrByteR       (const ContainerT& aContainer): GpRawPtrR(aContainer.data(), count_t::SMake(aContainer.size())) {}
};

class GpRawPtrByteRW: public GpRawPtrRW<std::byte*>
{
public:
    using this_type = GpRawPtrByteRW;

public:
                            GpRawPtrByteRW      (void) = delete;
    constexpr               GpRawPtrByteRW      (this_type& aRawPtr) noexcept: GpRawPtrRW(aRawPtr) {}
    constexpr               GpRawPtrByteRW      (this_type&& aRawPtr) noexcept: GpRawPtrRW(std::move(aRawPtr)) {}
    constexpr               GpRawPtrByteRW      (std::byte* aBytes, const size_byte_t aSize): GpRawPtrRW(aBytes, aSize) {}
    constexpr               GpRawPtrByteRW      (std::byte* aBytes, const count_t aCount): GpRawPtrRW(aBytes, aCount) {}
                            GpRawPtrByteRW      (std::string& aStr): GpRawPtrRW(reinterpret_cast<std::byte*>(aStr.data()), count_t::SMake(aStr.size())) {}
                            GpRawPtrByteRW      (GpBytesArray& aBytes): GpRawPtrRW(aBytes.data(), count_t::SMake(aBytes.size())) {}

    template<typename ContainerT>
    constexpr               GpRawPtrByteRW      (ContainerT& aContainer): GpRawPtrRW(aContainer.data(), count_t::SMake(aContainer.size())) {}

    template<typename D>
    constexpr void          CopyFrom            (const D& aRawPtr);

    inline void             CopyFrom            (const std::byte* aPtr, const count_t aCount);
    inline void             CopyFrom            (const std::byte* aPtr, const size_byte_t aSize);
    inline void             CopyFrom            (const char* aPtr, const count_t aCount);
    inline void             CopyFrom            (const char* aPtr, const size_byte_t aSize);
};

template<typename D>
constexpr void  GpRawPtrByteRW::CopyFrom (const D& aRawPtr)
{
    GpRawPtrRW::CopyFrom<D>(aRawPtr);
}

void    GpRawPtrByteRW::CopyFrom (const std::byte* aPtr, const count_t aCount)
{
    GpRawPtrRW::CopyFrom(aPtr, aCount);
}

void    GpRawPtrByteRW::CopyFrom (const std::byte* aPtr, const size_byte_t aSize)
{
    GpRawPtrRW::CopyFrom(aPtr, aSize.ValueAs<count_t>());
}

void    GpRawPtrByteRW::CopyFrom (const char* aPtr, const count_t aCount)
{
    GpRawPtrRW::CopyFrom(reinterpret_cast<const std::byte*>(aPtr), aCount);
}

void    GpRawPtrByteRW::CopyFrom (const char* aPtr, const size_byte_t aSize)
{
    GpRawPtrRW::CopyFrom(reinterpret_cast<const std::byte*>(aPtr), aSize.ValueAs<count_t>());
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
