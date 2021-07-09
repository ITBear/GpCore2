#pragma once

#include "GpRandomDeviceIf.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)
#   if defined(GP_OS_WINDOWS)

namespace GPlatform {

class GPCORE_API GpRandomDeviceWin: public GpRandomDeviceIf
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRandomDeviceWin)

public:
                                    GpRandomDeviceWin   (void) noexcept;
    virtual                         ~GpRandomDeviceWin  (void) noexcept override final;

    virtual result_type             operator()          (void) override final;

    static constexpr result_type    min                 (void) noexcept
    {
        return result_type(0);
    }

    static constexpr result_type    max                 (void) noexcept
    {
        static_assert(std::numeric_limits<result_type>::min() <= 0, "std::numeric_limits<result_type>::min() must be <= 0");
        static_assert(std::numeric_limits<result_type>::max() >= UINT_MAX, "std::numeric_limits<result_type>::max() must be >= UINT_MAX");
        return result_type(UINT_MAX);
    }

private:
    void                            CryptRefillRandom   (size_t aBufferSize);
    void                            Clear               (void) noexcept;

private:
    GpVector<u_int_8>               iRandomVec;
    size_t                          iRandomVecUnused    = 0;
};

}//namespace GPlatform

#   endif//#if defined(GP_OS_WINDOWS)
#endif//#if defined(GP_USE_RANDOM_GENERATORS)
