#include "GpRandom.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include <functional>

#   if defined(GP_OS_WINDOWS)
#       include "GpRandomDeviceWin.hpp"
#   endif//if defined(GP_OS_WINDOWS)

namespace GPlatform {

std::array<const std::string, 2> GpRandom::sStrs =
{
    std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"_sv),   //StrMode::ALPHA_NUM
    std::string("0123456789"_sv)                                                        //StrMode::NUM
};

void    GpRandom::SetSeedFromRD (void)
{
#if defined(GP_OS_WINDOWS)
    GpRandomDeviceWin rd;
#else
    std::random_device rd;
#endif

    std::array<random_mt19937::result_type, random_mt19937::state_size> seed_data;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    iEngine.seed(seq);
}

void    GpRandom::SetSeed (const random_mt19937::result_type aSeed)
{
    iEngine.seed(aSeed);
}

s_int_8 GpRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax) noexcept
{
    return Next<s_int_8>(aMin, aMax);
}

u_int_8 GpRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax) noexcept
{
    return Next<u_int_8>(aMin, aMax);
}

s_int_16    GpRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax) noexcept
{
    return Next<s_int_16>(aMin, aMax);
}

u_int_16    GpRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax) noexcept
{
    return Next<u_int_16>(aMin, aMax);
}

s_int_32    GpRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax) noexcept
{
    return Next<s_int_32>(aMin, aMax);
}

u_int_32    GpRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax) noexcept
{
    return Next<u_int_32>(aMin, aMax);
}

s_int_64    GpRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax) noexcept
{
    return Next<s_int_64>(aMin, aMax);
}

u_int_64    GpRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax) noexcept
{
    return Next<u_int_64>(aMin, aMax);
}

bool    GpRandom::Bool (void) noexcept
{
    return Next<u_int_64>(0, 1) == 0;
}

std::string     GpRandom::String
(
    const GpRandomStrMode::EnumT    aMode,
    const size_t                    aSize
)
{
    std::string_view    srcStr      = sStrs.at(size_t(aMode));
    const u_int_8       srcStrMaxId = NumOps::SConvert<u_int_8>(srcStr.size()) - 1;
    std::string         res;

    res.resize(aSize);
    char* strData = res.data();

    for (size_t id = 0; id < aSize; ++id)
    {
        *strData++ = srcStr.at(UI8(0, srcStrMaxId));
    }

    return res;
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
