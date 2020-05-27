#include "GpUUID.hpp"

#if defined(GP_USE_UUID)

#include "../../RandomGenerators/GpSRandom.hpp"

namespace GPlatform {

std::string	GpUUID::ToString (void) const
{
	//    8      4    4    4       12
	//xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
	std::string str;
	str.resize(36);
	char* strPtr = str.data();

	for (size_t id = 0; id < sizeof(RawT); ++id)
	{
		GpArray<char,2> h = GpStringOps::SFromByte(std::byte(*strPtr++));
		*strPtr++ = h.at(0);
		*strPtr++ = h.at(1);

		if ((id == 3) ||
			(id == 5) ||
			(id == 7) ||
			(id == 9))
		{
			*strPtr++ = '-';
		}
	}

	return str;
}

void	GpUUID::FromString (std::string_view aStr)
{
	if (aStr.length() != 36)
	{
		THROW_GPE("Length of UUID string must be 36"_sv);
	}

	const char*	strPtr = aStr.data();
	RawT		buff;

	for (size_t id = 0; id < sizeof(RawT); id++)
	{
		if ((id == 4) ||
			(id == 6) ||
			(id == 8) ||
			(id == 10))
		{
			strPtr++;
		}

		GpArray<char,2> str;
		str.data()[0] = *strPtr++;
		str.data()[1] = *strPtr++;

		buff.data()[id] = GpStringOps::SToByte(str);
	}

	std::memcpy(iRawData.data(), buff.data(), sizeof(RawT));
}

bool	GpUUID::IsEqual (const GpUUID& aUUID) const noexcept
{
	return std::memcmp(iRawData.data(), aUUID.iRawData.data(), iRawData.size()) == 0;
}

bool	GpUUID::IsZero (void) const noexcept
{
	RawT zero;
	std::memset(zero.data(), 0, zero.size());
	return std::memcmp(iRawData.data(), zero.data(), iRawData.size()) == 0;
}

void	GpUUID::Zero (void) noexcept
{
	std::memset(iRawData.data(), 0, iRawData.size());
}

void	GpUUID::SetRandom (GpRandom& aRandom)
{
	const u_int_64 part_0 = aRandom.UI64();
	const u_int_64 part_1 = aRandom.UI64();

	std::memcpy(iRawData.data() + 0*sizeof(u_int_64), &part_0, sizeof(u_int_64));
	std::memcpy(iRawData.data() + 1*sizeof(u_int_64), &part_1, sizeof(u_int_64));
}

GpUUID	GpUUID::SGenRandom (void)
{
	GpSRandom& rnd = GpSRandom::S();

	const u_int_64 part_0 = rnd.UI64();
	const u_int_64 part_1 = rnd.UI64();

	RawT rawData;

	std::memcpy(rawData.data() + 0*sizeof(u_int_64), &part_0, sizeof(u_int_64));
	std::memcpy(rawData.data() + 1*sizeof(u_int_64), &part_1, sizeof(u_int_64));

	return GpUUID(rawData);
}

GpUUID	GpUUID::SFromString (std::string_view aStr)
{
	GpUUID uuid;

	uuid.FromString(aStr);

	return uuid;
}

}//GPlatform

#endif//#if defined(GP_USE_UUID)
