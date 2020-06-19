#include "GpStringOps.hpp"

#if defined(GP_USE_STRINGS)

#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitOps.hpp"

namespace GPlatform {

const GpArray<char, 201> GpStringOps::sDigits =
{
	"0001020304050607080910111213141516171819"
	"2021222324252627282930313233343536373839"
	"4041424344454647484950515253545556575859"
	"6061626364656667686970717273747576777879"
	"8081828384858687888990919293949596979899"
};

GpVector<std::string_view>	GpStringOps::SSplit (std::string_view		aSourceStr,
												 const char				aDelim,
												 const count_t			aReturnPartsCountLimit,
												 const count_t			aDelimCountLimit,
												 const Algo::SplitMode	aSplitMode)
{
	auto a = GpRawPtrCharR(aSourceStr);
	auto b = GpRawPtrCharR(&aDelim, 1_cnt);

	return Algo::Split<char, GpVector<std::string_view>>(a,
														 b,
														 aReturnPartsCountLimit,
														 aDelimCountLimit,
														 aSplitMode);
}

GpVector<std::string_view>	GpStringOps::SSplit (std::string_view		aSourceStr,
												 std::string_view		aDelim,
												 const count_t			aReturnPartsCountLimit,
												 const count_t			aDelimCountLimit,
												 const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string_view>>(GpRawPtrCharR(aSourceStr),
														 GpRawPtrCharR(aDelim),
														 aReturnPartsCountLimit,
														 aDelimCountLimit,
														 aSplitMode);
}

count_t	GpStringOps::SFromUI64 (const UInt64	aValue,
								GpRawPtrCharRW	aStrOut)
{
	const count_t length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(aValue.ValueAs<u_int_64>()));

	THROW_GPE_COND_CHECK_M(aStrOut.LengthLeft() >= length, "aStrOut size are too small"_sv);

	_SFromUI64(aValue, GpRawPtrCharRW(aStrOut.Ptr(), length));

	return length;
}

std::string	GpStringOps::SFromUI64 (const UInt64 aValue)
{
	const size_t length = GpNumericOps::SDecDigsCountUI64(aValue.ValueAs<u_int_64>());
	std::string s;
	s.resize(length);

	_SFromUI64(aValue, GpRawPtrCharRW(s));

	return s;
}

count_t	GpStringOps::SFromSI64 (const SInt64	aValue,
								GpRawPtrCharRW	aStrOut)
{
	count_t length = 0_cnt;

	if (aValue >= 0_s_int_64)
	{
		const UInt64 v = UInt64::SBitCast(aValue);
		length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v.ValueAs<u_int_64>()));

		THROW_GPE_COND_CHECK_M(aStrOut.LengthLeft() >= length, "aStrOut size are too small"_sv);

		_SFromUI64(v, GpRawPtrCharRW(aStrOut.Ptr(), length));
	} else
	{
		const UInt64 v = UInt64::SBitCast(-aValue);
		length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v.ValueAs<u_int_64>()));

		THROW_GPE_COND_CHECK_M(aStrOut.LengthLeft() >= (length + 1_cnt), "StrOut size are too small"_sv);

		*aStrOut++ = '-';
		_SFromUI64(v, GpRawPtrCharRW(aStrOut.Ptr(), length));
		length++;
	}

	return length;
}

std::string	GpStringOps::SFromSI64 (const SInt64 aValue)
{
	std::string	s;
	count_t length = 0_cnt;

	if (aValue >= 0_s_int_64)
	{
		const UInt64 v = UInt64::SBitCast(aValue);
		length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v.ValueAs<u_int_64>()));

		s.resize(length.ValueAs<size_t>());

		_SFromUI64(v, GpRawPtrCharRW(s.data(), length));
	} else
	{
		const UInt64 v = UInt64::SBitCast(-aValue);
		length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v.ValueAs<u_int_64>()));

		s.resize((length + 1_cnt).ValueAs<size_t>());

		*s.data() = '-';
		_SFromUI64(v, GpRawPtrCharRW(s.data() + 1, length));
		length++;
	}

	return s;
}

count_t	GpStringOps::SFromDouble (const double		aValue,
								  GpRawPtrCharRW	aStrOut)
{
	//TODO reimplement
	std::string s = std::to_string(aValue);

	const count_t length = count_t::SMake(s.length());

	THROW_GPE_COND_CHECK_M(aStrOut.LengthLeft() >= length, "aMaxOutLength value are too small"_sv);

	SReplace(s, ',', '.');

	MemOps::SCopy(aStrOut.Ptr(), s.data(), length);

	return length;
}

std::string	GpStringOps::SFromDouble (const double aValue)
{
	//TODO reimplement
	std::string s = std::to_string(aValue);
	SReplace(s, ',', '.');
	return s;
}

UInt64	GpStringOps::SToUI64 (std::string_view aStr)
{
	GpRawPtrCharR	str(aStr);
	UInt64			res	= 0_u_int_64;

	while (str.LengthLeft() > 0_cnt)
	{
		const char ch = *str++;

		if ((ch >= '0') && (ch <= '9'))
		{
			res = (res * UInt64::SMake(10)) + UInt64::SMake(u_int_64(ch) - u_int_64('0'));
		} else
		{
			THROW_GPE("Value "_sv + aStr + " contains wrong character '"_sv + ch + "'"_sv);
		}
	}

	return res;
}

SInt64	GpStringOps::SToSI64 (std::string_view aStr)
{
	GpRawPtrCharR str(aStr);

	const char signChar = *str;
	SInt64 sign = 1_s_int_64;

	if (signChar == '-')
	{
		++str;
		sign = -1_s_int_64;
	} else if (signChar == '+')
	{
		++str;
		sign = 1_s_int_64;
	}

	const UInt64 valueWithoutSign = SToUI64(str.AsStringView());

	if (sign >= 0_s_int_64)
	{
		if (valueWithoutSign > UInt64::SMake(9223372036854775807ULL))
		{
			THROW_GPE("Value "_sv + aStr + " is out of range SInt64"_sv);
		} else
		{
			return SInt64::SBitCast(valueWithoutSign);
		}
	} else
	{
		if (valueWithoutSign > UInt64::SMake(9223372036854775808ULL))
		{
			THROW_GPE("Value "_sv + aStr + " is out of range SInt64"_sv);
		} else if (valueWithoutSign == UInt64::SMake(9223372036854775808ULL))
		{
			return SInt64::SBitCast(valueWithoutSign);//trick
		} else
		{
			return SInt64::SBitCast(valueWithoutSign) * sign;
		}
	}
}

double		GpStringOps::SToDouble_fast (std::string_view aStr)
{
	//TODO: reimplement with http://www.netlib.org/fp/. Use dtoa.c
	//Supported format: [+-][UInt64][.[UInt64]]

	//----------------- Find decimal separator (radix character) -----------------
	GpVector<std::string_view> parts = SSplit(aStr, '.', 2_cnt, 1_cnt, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);
	std::string_view integerPart;
	std::string_view fractionalPart;

	if (parts.size() == 1)
	{
		integerPart = aStr;
	} else
	{
		integerPart		= parts.at(0);
		fractionalPart	= parts.at(1);
	}

	//----------------- Parse integer and fractional parts -----------------
	double res	= 0.0;
	double sign	= 1.0;

	if (!integerPart.empty())
	{
		const char signChar = integerPart.at(0);
		if (signChar == '-')
		{
			sign = -1.0;
			integerPart = std::string_view(integerPart.data() + 1, integerPart.size() - 1);
		} else if (signChar == '+')
		{
			sign = 1.0;
			integerPart = std::string_view(integerPart.data() + 1, integerPart.size() - 1);
		}

		const UInt64 v = (!integerPart.empty()) ? SToUI64(integerPart) : 0_u_int_64;
		res = double(v.Value());
	}

	//Parse fractional part (to the right of the radix point)
	if (!fractionalPart.empty())
	{
		const UInt64 v = SToUI64(fractionalPart);
		res += double(v.Value())/pow(10.0, double(fractionalPart.size()));
	}

	return res*sign;
}

std::variant<SInt64, double>	GpStringOps::SToNumeric (std::string_view aStr)
{
	//[+-][UInt64][.[UInt64]] - DOUBLE
	//[+-]digits - INT

	if (aStr.find_first_of('.') == std::string::npos)//int
	{
		SInt64 val = SToSI64(aStr);
		return {val};
	} else //double
	{
		double val = SToDouble_fast(aStr);
		return {val};
	}
}

count_t	GpStringOps::SFromBytes (GpRawPtrByteR	aData,
								 GpRawPtrCharRW	aStrOut)
{
	const count_t dataLength	= aData.LengthLeft();
	const count_t strOutLength	= aStrOut.LengthLeft();

	THROW_GPE_COND_CHECK_M(strOutLength >= (dataLength * 2_cnt), "Out string size is too small"_sv);

	while (aData.LengthLeft() > 0_cnt)
	{
		const size_t b	= size_t(*aData++);
		const size_t lo	= (b & size_t(0x0F)) >> 0;
		const size_t hi	= (b & size_t(0xF0)) >> 4;

		*aStrOut++ = char((hi < size_t(10)) ? (size_t('0')+ hi) : (size_t('a') + (hi - size_t(10))));
		*aStrOut++ = char((lo < size_t(10)) ? (size_t('0')+ lo) : (size_t('a') + (lo - size_t(10))));
	}

	return dataLength * 2_cnt;
}

std::string	GpStringOps::SFromBytes (const GpBytesArray& aData)
{
	return SFromBytes(GpRawPtrByteR(aData));
}

std::string	GpStringOps::SFromBytes (GpRawPtrByteR aData)
{
	const count_t charsCount = count_t::SMake(aData.SizeLeft().Value()) * 2_cnt;

	std::string res;
	res.resize(charsCount.ValueAs<size_t>());

	GpRawPtrCharRW resPtr(res);

	if (SFromBytes(aData, resPtr) != charsCount)
	{
		THROW_GPE("Failed to convert bytes to hex string"_sv);
	}

	return res;
}

std::string	GpStringOps::SFromBytes (std::string_view aData)
{
	return SFromBytes(GpRawPtrByteR(aData));
}

size_byte_t	GpStringOps::SToBytes (std::string_view	aStr,
								   GpRawPtrByteRW	aDataOut)
{
	//String length
	const count_t strLength = count_t::SMake(aStr.length());

	if (strLength == 0_cnt)
	{
		return 0_byte;
	}

	THROW_GPE_COND_CHECK_M((strLength % 2_cnt) == 0_cnt, "String length must be even"_sv);

	//Remove prefix
	std::string_view prefix = aStr.substr(0, 2);
	std::string_view strHex	= aStr;
	if ((prefix == "0x"_sv) ||
		(prefix == "0X"_sv))
	{
		strHex = aStr.substr(2, aStr.size() - 2);
	}

	//
	GpRawPtrCharR strHexPtr(strHex);

	const count_t outSize = strHexPtr.LengthLeft() / 2_cnt;
	THROW_GPE_COND_CHECK_M(aDataOut.CountLeft() >= outSize, "Out data size is too small"_sv);

	while (strHexPtr.LengthLeft() > 0_cnt)
	{
		GpArray<char,2>	s = {*strHexPtr++, *strHexPtr++};
		*aDataOut++ = SToByte(s);
	}

	return size_byte_t::SMake(outSize.Value());
}

size_byte_t	GpStringOps::SToBytes (std::string_view	aStr,
								   GpBytesArray&	aDataOut)
{
	//String length
	const count_t strLength = count_t::SMake(aStr.length());

	if (strLength == 0_cnt)
	{
		return 0_byte;
	}

	THROW_GPE_COND_CHECK_M((strLength % 2_cnt) == 0_cnt, "String length must be even"_sv);

	//Remove prefix
	std::string_view prefix = aStr.substr(0, 2);
	std::string_view strHex	= aStr;
	if ((prefix == "0x"_sv) ||
		(prefix == "0X"_sv))
	{
		strHex = aStr.substr(2, aStr.size() - 2);
	}

	//
	GpRawPtrCharR strHexPtr(strHex);

	const count_t oldSize	= count_t::SMake(aDataOut.size());
	const count_t outSize	= strHexPtr.LengthLeft() / 2_cnt;
	aDataOut.resize((oldSize + outSize).ValueAs<size_t>());

	GpRawPtrByteRW dataOut(aDataOut.data() + oldSize.ValueAs<size_t>(), size_byte_t::SMake(outSize.Value()));

	while (strHexPtr.LengthLeft() > 0_cnt)
	{
		GpArray<char,2>	s = {*strHexPtr++, *strHexPtr++};
		*dataOut++ = SToByte(s);
	}

	return size_byte_t::SMake(outSize.Value());
}

GpBytesArray	GpStringOps::SToBytes (std::string_view aStr)
{
	GpBytesArray res;
	SToBytes(aStr, res);
	return res;
}

count_t		GpStringOps::SConv_UTF16_UTF8 (GpArray<std::byte, 4>&		aUTF8_valueOut,
										   const GpArray<std::byte, 2>	aUTF16_value)
{
	const u_int_16	utf16_val	= BitOps::N2H(std::bit_cast<u_int_16>(aUTF16_value));
	std::byte*		utf8_val	= aUTF8_valueOut.data();

	//Convert to UTF8
	if (size_t(utf16_val) < size_t(0x00000080ULL))//0x00000000 — 0x0000007F
	{
		//0xxxxxxx
		*utf8_val++ = std::byte(utf16_val);
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		return 1_cnt;
	} else if (size_t(utf16_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
	{
		//110xxxxx 10xxxxxx
		*utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf16_val) >> 6) & size_t(0x1F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0) & size_t(0x3F)));
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		return 2_cnt;
	} else
	{
		//1110xxxx 10xxxxxx 10xxxxxx
		*utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf16_val) >> 12) & size_t(0x0F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 6)  & size_t(0x3F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0)  & size_t(0x3F)));
		*utf8_val++ = std::byte(0);
		return 3_cnt;
	}
}

count_t		GpStringOps::SConv_UTF32_UTF8 (GpArray<std::byte, 4>&		aUTF8_valueOut,
										   const GpArray<std::byte, 4>	aUTF32_value)
{
	const u_int_32	utf32_val	= BitOps::N2H(std::bit_cast<u_int_32>(aUTF32_value));
	std::byte*		utf8_val	= aUTF8_valueOut.data();

	//Convert to UTF8
	if (size_t(utf32_val) < size_t(0x00000080ULL))//0x00000000 — 0x0000007F
	{
		//0xxxxxxx
		*utf8_val++ = std::byte(utf32_val);
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		return 1_cnt;
	} else if (size_t(utf32_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
	{
		//110xxxxx 10xxxxxx
		*utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf32_val) >> 6) & size_t(0x1F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0) & size_t(0x3F)));
		*utf8_val++ = std::byte(0);
		*utf8_val++ = std::byte(0);
		return 2_cnt;
	} else if (size_t(utf32_val) < size_t(0x00010000ULL))//0x00000800 — 0x0000FFFF
	{
		//1110xxxx 10xxxxxx 10xxxxxx
		*utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf32_val) >> 12) & size_t(0x0F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
		*utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
		*utf8_val++ = std::byte(0);
		return 3_cnt;
	} else if (size_t(utf32_val) < size_t(0x00200000ULL))//0x00010000 — 0x001FFFFF
	{
		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		utf8_val[0] = std::byte(size_t(0xF0) + ((size_t(utf32_val) >> 18) & size_t(0x07)));
		utf8_val[1] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 12) & size_t(0x3F)));
		utf8_val[2] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
		utf8_val[3] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
		return 4_cnt;
	} else
	{
		THROW_GPE("Wrong UTF32 value"_sv);
	}
}

std::string&	GpStringOps::SReplace (std::string& aStr, const char aChar, const char aNewChar)
{
	const size_t	len		= aStr.length();
	char* _R_		data	= aStr.data();

	for (size_t id = 0; id < len; ++id)
	{
		if (*data == aChar)
		{
			*data++ = aNewChar;
		} else
		{
			++data;
		}
	}

	return aStr;
}

void	GpStringOps::_SFromUI64 (const UInt64	aValue,
								 GpRawPtrCharRW	aStrOut)
{
	u_int_64		value	= aValue.ValueAs<u_int_64>();
	const char* _R_ digits	= sDigits.data();

	aStrOut += (aStrOut.LengthLeft() - 2_cnt);

	while (value >= 100)
	{
		const size_t i = size_t((value % u_int_64(100)) * 2);
		value /= 100;

		MemOps::SCopy(aStrOut.Ptr(), digits + i, 2_cnt);
		aStrOut -= 2_cnt;
	}

	// Handle last 1-2 digits
	if (value < 10)
	{
		*aStrOut = char(size_t('0') + size_t(value));
	} else
	{
		const size_t i = size_t(value) * 2;
		MemOps::SCopy(aStrOut.Ptr(), digits + i, 2_cnt);
	}
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
