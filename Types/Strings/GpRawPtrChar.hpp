#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "../Pointers/GpRawPtrR.hpp"
#include "../Pointers/GpRawPtrRW.hpp"

namespace GPlatform {

class GpRawPtrCharR: public GpRawPtrR<const char*>
{
public:
    using this_type = GpRawPtrCharR;

public:
                            GpRawPtrCharR       (void) = delete;
    constexpr               GpRawPtrCharR       (const this_type& aRawPtr) noexcept: GpRawPtrR(aRawPtr) {}
    constexpr               GpRawPtrCharR       (this_type&& aRawPtr) noexcept: GpRawPtrR(std::move(aRawPtr)) {}
    constexpr               GpRawPtrCharR       (const char* aStr, const count_t aCount): GpRawPtrR(aStr, aCount) {}
    constexpr               GpRawPtrCharR       (const char* aStr, const size_byte_t aSize): GpRawPtrR(aStr, aSize) {}
    constexpr               GpRawPtrCharR       (std::string_view aStr): GpRawPtrR(aStr.data(), count_t::SMake(aStr.size())) {}
                            GpRawPtrCharR       (const std::string& aStr): GpRawPtrR(aStr.data(), count_t::SMake(aStr.size())) {}
};

class GpRawPtrCharRW: public GpRawPtrRW<char*>
{
public:
    using this_type = GpRawPtrCharRW;

public:
                            GpRawPtrCharRW      (void) = delete;
    constexpr               GpRawPtrCharRW      (this_type& aRawPtr) noexcept: GpRawPtrRW(aRawPtr) {}
    constexpr               GpRawPtrCharRW      (this_type&& aRawPtr) noexcept: GpRawPtrRW(std::move(aRawPtr)) {}
    constexpr               GpRawPtrCharRW      (char* aStr, const count_t aCount): GpRawPtrRW(aStr, aCount) {}
    constexpr               GpRawPtrCharRW      (char* aStr, const size_byte_t aSize): GpRawPtrRW(aStr, aSize) {}
                            GpRawPtrCharRW      (std::string& aStr): GpRawPtrRW(aStr.data(), count_t::SMake(aStr.size())) {}

    template<typename D>
    constexpr void          CopyFrom            (const D& aRawPtr);

    inline void             CopyFrom            (const char* aPtr, const count_t aCount);
    inline void             CopyFrom            (const char* aPtr, const size_byte_t aSize);
    inline void             CopyFrom            (const std::byte* aPtr, const count_t aCount);
    inline void             CopyFrom            (const std::byte* aPtr, const size_byte_t aSize);
};

template<typename D>
constexpr void  GpRawPtrCharRW::CopyFrom (const D& aRawPtr)
{
    GpRawPtrRW::CopyFrom<D>(aRawPtr);
}

void    GpRawPtrCharRW::CopyFrom (const char* aPtr, const count_t aCount)
{
    GpRawPtrRW::CopyFrom(aPtr, aCount);
}

void    GpRawPtrCharRW::CopyFrom (const char* aPtr, const size_byte_t aSize)
{
    GpRawPtrRW::CopyFrom(aPtr, aSize.ValueAs<count_t>());
}

void    GpRawPtrCharRW::CopyFrom (const std::byte* aPtr, const count_t aCount)
{
    GpRawPtrRW::CopyFrom(reinterpret_cast<const char*>(aPtr), aCount);
}

void    GpRawPtrCharRW::CopyFrom (const std::byte* aPtr, const size_byte_t aSize)
{
    GpRawPtrRW::CopyFrom(reinterpret_cast<const char*>(aPtr), aSize.ValueAs<count_t>());
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
