#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "../Pointers/GpRawPtrR.hpp"
#include "../Pointers/GpRawPtrRW.hpp"

namespace GPlatform {

class GpRawPtrCharR: public GpRawPtrR<const char*>
{
public:
    using this_type	= GpRawPtrCharR;

public:
                        GpRawPtrCharR		(void) = delete;
    constexpr			GpRawPtrCharR		(const this_type& aRawPtr) noexcept: GpRawPtrR(aRawPtr) {}
    constexpr			GpRawPtrCharR		(this_type&& aRawPtr) noexcept: GpRawPtrR(std::move(aRawPtr)) {}
    constexpr			GpRawPtrCharR		(const char* aStr, const count_t aCount): GpRawPtrR(aStr, aCount) {}
    constexpr			GpRawPtrCharR		(std::string_view aStr): GpRawPtrR(aStr.data(), count_t::SMake(aStr.size())) {}
                        GpRawPtrCharR		(const std::string& aStr): GpRawPtrR(aStr.data(), count_t::SMake(aStr.size())) {}
};

class GpRawPtrCharRW: public GpRawPtrRW<char*>
{
public:
    using this_type	= GpRawPtrCharRW;

public:
                        GpRawPtrCharRW		(void) = delete;
    constexpr			GpRawPtrCharRW		(const this_type& aRawPtr) noexcept: GpRawPtrRW(aRawPtr) {}
    constexpr			GpRawPtrCharRW		(this_type&& aRawPtr) noexcept: GpRawPtrRW(std::move(aRawPtr)) {}
    constexpr			GpRawPtrCharRW		(char* aStr, const count_t aCount): GpRawPtrRW(aStr, aCount) {}
                        GpRawPtrCharRW		(std::string& aStr): GpRawPtrRW(aStr.data(), count_t::SMake(aStr.size())) {}
};

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
