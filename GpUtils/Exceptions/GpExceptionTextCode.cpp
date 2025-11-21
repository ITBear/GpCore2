#include <GpCore2/GpUtils/Exceptions/GpExceptionTextCode.hpp>

#if defined(GP_USE_EXCEPTIONS)

namespace GPlatform {

GpExceptionTextCode::~GpExceptionTextCode (void) noexcept
{
//__attribute__((availability(macos, strict, introduced = 13.3)))

/*
     __attribute__((availability(macos, strict, introduced = 13.3)))                                               \
    __attribute__((availability(ios, strict, introduced = 16.3)))                                                 \
    __attribute__((availability(tvos, strict, introduced = 16.3)))                                                \
    __attribute__((availability(watchos, strict, introduced = 9.3)))                                              \
    __attribute__((availability(bridgeos, strict, introduced = 7.5)))

 */
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
