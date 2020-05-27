#include "GpRandomDeviceWin.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)
#	if defined(GP_OS_WINDOWS)

#include <Wincrypt.h>

namespace GPlatform {

GpRandomDeviceWin::GpRandomDeviceWin (void) noexcept
{
}

GpRandomDeviceWin::~GpRandomDeviceWin (void) noexcept
{
	Clear();
}

GpRandomDeviceWin::result_type	GpRandomDeviceWin::operator() (void)
{
	/*unsigned int res = 0;
	if (rand_s(&res) != 0)
	{
		THROW_GP_EXCEPTION("Random device (rand_s) return error"_sv);
	}

	return result_type(res);*/

	if (iRandomVecUnused < sizeof(result_type))
	{
		CryptRefillRandom(sizeof(result_type)*32);
	}

	if (iRandomVecUnused < sizeof(result_type))
	{
		THROW_GP_EXCEPTION("iRandomVecUnused < sizeof(result_type)");
	}

	result_type res;

	std::memcpy(&res, iRandomVec.data() + iRandomVec.size() - iRandomVecUnused, sizeof(result_type));

	iRandomVecUnused -= sizeof(result_type);

	return res;
}

void    GpRandomDeviceWin::CryptRefillRandom (size_t aBufferSize)
{
	Clear();

	HCRYPTPROV hProvider = 0;

	if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
	{
		THROW_GP_EXCEPTION("CryptAcquireContextW return error: "_sv + GpErrno::WinGetLastErrorAsString());
	}

	const DWORD dwLength = DWORD(aBufferSize);
	iRandomVec.resize(aBufferSize);

	if (!::CryptGenRandom(hProvider, dwLength, iRandomVec.data()))
	{
		Clear();
		::CryptReleaseContext(hProvider, 0);
		THROW_GP_EXCEPTION("CryptGenRandom return error: "_sv + GpErrno::WinGetLastErrorAsString());
	}

	if (!::CryptReleaseContext(hProvider, 0))
	{
		Clear();
		THROW_GP_EXCEPTION("CryptReleaseContext return error: "_sv + GpErrno::WinGetLastErrorAsString());
	}

	iRandomVecUnused = aBufferSize;
}

void	GpRandomDeviceWin::Clear (void) noexcept
{
	const size_t	s = iRandomVec.size();
	u_int_8*		p = iRandomVec.data();

	for (size_t id = 0; id < s; ++id)
	{
		*p++ = u_int_8(0xFF);
	}

	iRandomVecUnused = 0;
}

}//namespace GPlatform

#	endif//#if defined(GP_OS_WINDOWS)
#endif//#if defined(GP_USE_RANDOM_GENERATORS)
