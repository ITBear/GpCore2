#include <GpCore2/GpUtils/Random/GpRandomDeviceWin.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#if defined(GP_USE_RANDOM_GENERATORS) && defined(GP_OS_WINDOWS)

#include <GpCore2/Config/IncludeExt/windows.hpp>
#include <wincrypt.h>

#include <GpCore2/GpUtils/Other/GpErrno.hpp>

namespace GPlatform {

GpRandomDeviceWin::GpRandomDeviceWin (void) noexcept
{
}

GpRandomDeviceWin::~GpRandomDeviceWin (void) noexcept
{
    Clear();
}

GpRandomDeviceWin::result_type  GpRandomDeviceWin::operator() (void)
{
    if (iRandomVecUnused < sizeof(result_type))
    {
        CryptRefillRandom(sizeof(result_type)*32);
    }

    if (iRandomVecUnused < sizeof(result_type))
    {
        THROW_GP("iRandomVecUnused < sizeof(result_type)");
    }

    result_type res;

    std::memcpy(&res, std::data(iRandomVec) + std::size(iRandomVec) - iRandomVecUnused, sizeof(result_type));

    iRandomVecUnused -= sizeof(result_type);

    return res;
}

void    GpRandomDeviceWin::CryptRefillRandom (size_t aBufferSize)
{
    Clear();

    HCRYPTPROV hProvider = 0;

    if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    {
        THROW_GP("CryptAcquireContextW return error: "_sv + GpErrno::SWinGetAndClear());
    }

    const DWORD dwLength = DWORD(aBufferSize);
    iRandomVec.resize(aBufferSize);

    if (!::CryptGenRandom(hProvider, dwLength, std::data(iRandomVec)))
    {
        Clear();
        ::CryptReleaseContext(hProvider, 0);
        THROW_GP("CryptGenRandom return error: "_sv + GpErrno::SWinGetAndClear());
    }

    if (!::CryptReleaseContext(hProvider, 0))
    {
        Clear();
        THROW_GP("CryptReleaseContext return error: "_sv + GpErrno::SWinGetAndClear());
    }

    iRandomVecUnused = aBufferSize;
}

void    GpRandomDeviceWin::Clear (void) noexcept
{
    const size_t    s = std::size(iRandomVec);
    u_int_8* _R_    p = std::data(iRandomVec);

    for (size_t id = 0; id < s; ++id)
    {
        *p++ = u_int_8(0xFF);
    }

    iRandomVecUnused = 0;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_RANDOM_GENERATORS) && defined(GP_OS_WINDOWS)
