#include "GpStringOps.hpp"
#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitOps.hpp"

#if defined(GP_USE_STRINGS)

namespace GPlatform {

const GpArray<char, 201> GpStringOps::sDigits =
{
	"0001020304050607080910111213141516171819"
	"2021222324252627282930313233343536373839"
	"4041424344454647484950515253545556575859"
	"6061626364656667686970717273747576777879"
	"8081828384858687888990919293949596979899"
};

GpVector<std::string>	GpStringOps::SSplitToStr (std::string_view		aSourceStr,
												  const char			aDelim,
												  const count_t			aReturnPartsCountLimit,
												  const count_t			aDelimCountLimit,
												  const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string>>(aSourceStr.data(),
													count_t::SMake(aSourceStr.size()),
													&aDelim,
													1_cnt,
													aReturnPartsCountLimit,
													aDelimCountLimit,
													aSplitMode);
}

GpVector<std::string>	GpStringOps::SSplitToStr (std::string_view		aSourceStr,
												  std::string_view		aDelim,
												  const count_t			aReturnPartsCountLimit,
												  const count_t			aDelimCountLimit,
												  const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string>>(aSourceStr.data(),
													count_t::SMake(aSourceStr.size()),
													aDelim.data(),
													count_t::SMake(aDelim.size()),
													aReturnPartsCountLimit,
													aDelimCountLimit,
													aSplitMode);
}

GpVector<std::string>	GpStringOps::SSplitToStr (std::string_view		aSourceStr,
												  const char			aDelim,
												  const char			aEscGroupBegin,
												  const char			aEscGroupEnd,
												  const count_t			aReturnPartsCountLimit,
												  const count_t			aDelimCountLimit,
												  const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string>>(aSourceStr.data(),
													count_t::SMake(aSourceStr.size()),
													aDelim,
													aEscGroupBegin,
													aEscGroupEnd,
													aReturnPartsCountLimit,
													aDelimCountLimit,
													aSplitMode);
}

GpVector<std::string_view>	GpStringOps::SSplitToView (std::string_view			aSourceStr,
													   const char				aDelim,
													   const count_t			aReturnPartsCountLimit,
													   const count_t			aDelimCountLimit,
													   const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string_view>>(aSourceStr.data(),
														 count_t::SMake(aSourceStr.size()),
														 &aDelim,
														 1_cnt,
														 aReturnPartsCountLimit,
														 aDelimCountLimit,
														 aSplitMode);
}

GpVector<std::string_view>	GpStringOps::SSplitToView (std::string_view			aSourceStr,
													   std::string_view			aDelim,
													   const count_t			aReturnPartsCountLimit,
													   const count_t			aDelimCountLimit,
													   const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string_view>>(aSourceStr.data(),
														 count_t::SMake(aSourceStr.size()),
														 aDelim.data(),
														 count_t::SMake(aDelim.size()),
														 aReturnPartsCountLimit,
														 aDelimCountLimit,
														 aSplitMode);
}

GpVector<std::string_view>	GpStringOps::SSplitToView (std::string_view			aSourceStr,
													   const char				aDelim,
													   const char				aEscGroupBegin,
													   const char				aEscGroupEnd,
													   const count_t			aReturnPartsCountLimit,
													   const count_t			aDelimCountLimit,
													   const Algo::SplitMode	aSplitMode)
{
	return Algo::Split<char, GpVector<std::string_view>>(aSourceStr.data(),
														 count_t::SMake(aSourceStr.size()),
														 aDelim,
														 aEscGroupBegin,
														 aEscGroupEnd,
														 aReturnPartsCountLimit,
														 aDelimCountLimit,
														 aSplitMode);
}

count_t	GpStringOps::SFromUI64 (const u_int_64	aValue,
								char*			aStrOut,
								const count_t	aMaxOutLength)
{
	const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);

	THROW_GPE_COND_CHECK_M(aMaxOutLength.ValueAs<size_t>() >= length, "aMaxOutLength value are too small"_sv);

	_SFromUI64(aValue, aStrOut, count_t::SMake(length));

	return count_t::SMake(length);
}

std::string	GpStringOps::SFromUI64 (const u_int_64 aValue)
{
	const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);
	std::string s;
	s.resize(length);

	_SFromUI64(aValue, s.data(), count_t::SMake(length));

	return s;
}

count_t	GpStringOps::SFromSI64 (const s_int_64	aValue,
								char*			aStrOut,
								const count_t	aMaxOutLength)
{
	size_t length = 0;
	if (aValue >= 0)
	{
		const u_int_64 v = static_cast<u_int_64>(aValue);
		length = GpNumericOps::SDecDigsCountUI64(v);

		THROW_GPE_COND_CHECK_M(aMaxOutLength.ValueAs<size_t>() >= length, "aMaxOutLength value are too small"_sv);

		_SFromUI64(v, aStrOut, count_t::SMake(length));
	} else
	{
		const u_int_64 v = static_cast<u_int_64>(-aValue);
		length = GpNumericOps::SDecDigsCountUI64(v);

		THROW_GPE_COND_CHECK_M(aMaxOutLength.ValueAs<size_t>() >= (length + 1), "aMaxOutLength value are too small"_sv);

		aStrOut[0] = '-';
		_SFromUI64(v, aStrOut + 1, count_t::SMake(length));
		length++;
	}

	return count_t::SMake(length);
}

std::string	GpStringOps::SFromSI64 (const s_int_64 aValue)
{
	std::string	s;
	size_t		length = 0;

	if (aValue >= 0)
	{
		const u_int_64 v = static_cast<u_int_64>(aValue);
		length = GpNumericOps::SDecDigsCountUI64(v);
		s.resize(length);

		_SFromUI64(v, s.data(), count_t::SMake(length));
	} else
	{
		const u_int_64 v = static_cast<u_int_64>(-aValue);
		length = GpNumericOps::SDecDigsCountUI64(v);
		s.resize(length + 1);

		s.data()[0] = '-';
		_SFromUI64(v, s.data() + 1, count_t::SMake(length));
		length++;
	}

	return s;
}

count_t	GpStringOps::SFromDouble (const double	aValue,
								  char*			aStrOut,
								  const count_t	aMaxOutLength)
{
	//TODO reimplement
	std::string s = std::to_string(aValue);

	THROW_GPE_COND_CHECK_M(aMaxOutLength.ValueAs<size_t>() >= s.length(), "aMaxOutLength value are too small"_sv);

	SReplace(s, ',', '.');

	std::memcpy(aStrOut, s.data(), s.length());

	return count_t::SMake(s.length());
}

std::string	GpStringOps::SFromDouble (const double aValue)
{
	//TODO reimplement
	std::string s = std::to_string(aValue);
	SReplace(s, ',', '.');
	return s;
}

u_int_64	GpStringOps::SToUI64 (std::string_view aStr)
{
	const char*	_R_	strData = aStr.data();
	const size_t	strSize	= aStr.length();

	THROW_GPE_COND_CHECK_M((strData != nullptr) && (strSize != 0), "String size is 0"_sv);

	u_int_64 res = 0;

	for (size_t id = 0; id < strSize; ++id)
	{
		const char ch = *strData++;
		if ((ch >= '0') && (ch <= '9'))
		{
			u_int_64 old = res;
			res = (res * u_int_64(10)) + (u_int_64(ch) - u_int_64('0'));
			if (old > res)
			{
				THROW_GPE("Value "_sv + aStr + " is out of range u_int_64"_sv);
			}
		} else
		{
			THROW_GPE("Value "_sv + aStr + " contains wrong character '"_sv + ch + "'"_sv);
		}
	}

	return res;
}

s_int_64	GpStringOps::SToSI64 (std::string_view aStr)
{
	const char*	_R_	strData = aStr.data();
	const size_t	strSize	= aStr.length();

	THROW_GPE_COND_CHECK_M((strData != nullptr) && (strSize != 0), "String size is 0"_sv);

	const char			signChar = aStr.at(0);
	std::string_view	strWithoutSign;
	s_int_64			sign = 1;

	if (signChar == '-')
	{
		strWithoutSign	= std::string_view(strData + 1, strSize - 1);
		sign			= -1;
	} else if (signChar == '+')
	{
		strWithoutSign	= std::string_view(strData + 1, strSize - 1);
		sign			= 1;
	} else
	{
		strWithoutSign	= std::string_view(strData, strSize);
		sign			= 1;
	}

	const u_int_64 valueWithoutSign = (strWithoutSign.length() > 0) ? SToUI64(strWithoutSign) : 0;

	if (sign >= 0)
	{
		if (valueWithoutSign > u_int_64(9223372036854775807ULL))
		{
			THROW_GPE("Value "_sv + aStr + " is out of range s_int_64"_sv);
		}

		return s_int_64(valueWithoutSign);
	} else
	{
		if (valueWithoutSign > u_int_64(9223372036854775808ULL))
		{
			THROW_GPE("Value "_sv + aStr + " is out of range s_int_64"_sv);
		} else if (valueWithoutSign == u_int_64(9223372036854775808ULL))
		{
			return s_int_64(valueWithoutSign);//trick
		}

		return s_int_64(valueWithoutSign) * sign;
	}
}

double		GpStringOps::SToDouble_fast (std::string_view aStr)
{
	//TODO: reimplement with http://www.netlib.org/fp/. Use dtoa.c
	//Supported format: [+-][u_int_64][.[u_int_64]]

	const char* _R_	strData = aStr.data();
	const size_t	strSize	= aStr.size();

	THROW_GPE_COND_CHECK_M((strData != nullptr) && (strSize != 0), "String size is 0"_sv);

	//----------------- Find decimal separator (radix character) -----------------
	std::string_view integerPartSV = aStr;
	std::string_view fractionalPartSV;

	for (size_t id = 0; id < strSize; ++id)
	{
		if (strData[id] == '.')
		{
			if (id > 0)
			{
				integerPartSV = std::string_view(aStr.data(), id);
			} else
			{
				integerPartSV = std::string_view();
			}

			fractionalPartSV = std::string_view(aStr.data() + (id + 1),
												aStr.size() - (id + 1));

			break;
		}
	}

	//----------------- Parse integer and fractional parts -----------------
	double res	= 0.0;
	double sign	= 1.0;

	if (!integerPartSV.empty())
	{
		const char signChar = integerPartSV.at(0);
		if (signChar == '-')
		{
			sign = -1.0;
			integerPartSV = std::string_view(integerPartSV.data() + 1, integerPartSV.size() - 1);
		} else if (signChar == '+')
		{
			sign = 1.0;
			integerPartSV = std::string_view(integerPartSV.data() + 1, integerPartSV.size() - 1);
		}

		const u_int_64 integerPart = (!integerPartSV.empty()) ? SToUI64(integerPartSV) : 0;
		res = double(integerPart);
	}

	//Parse fractional part (to the right of the radix point)
	if (!fractionalPartSV.empty())
	{
		const u_int_64 fractionalPart = SToUI64(fractionalPartSV);
		res += double(fractionalPart)/pow(10.0, double(fractionalPartSV.size()));
	}

	return res*sign;
}

GpTuple<count_t, std::variant<s_int_64, double>>	GpStringOps::SToNumeric (std::string_view aStr)
{
	const char* _R_	strData		= aStr.data();
	size_t			strSize		= aStr.size();
	size_t			readChars	= 0;
	bool			isInt		= true;

	THROW_GPE_COND_CHECK_M((strData != nullptr) && (strSize != 0), "String size is 0"_sv);

	//---------------------------------------------
	//[+-][u_int_64][.[u_int_64]] - DOUBLE
	//[+-]digits - INT

	char ch = *strData++;
	readChars++;
	strSize--;

	if ((strSize > 0) &&
		((ch == '-') || (ch == '+')))
	{
		ch = *strData++;
		readChars++;
		strSize--;
	}

	while ((strSize > 0) &&
		   ((ch >= '0') && (ch <= '9')))
	{
		ch = *strData++;
		readChars++;
		strSize--;
	}

	if ((strSize > 0) &&
		(ch == '.'))
	{
		isInt = false;
		ch = *strData++;
		readChars++;
		strSize--;
	}

	while ((strSize > 0) &&
		   ((ch >= '0') && (ch <= '9')))
	{
		ch = *strData++;
		readChars++;
		strSize--;
	}

	readChars--;
	THROW_GPE_COND_CHECK_M(readChars != 0, "Unexpected character '"_sv + ch + "'"_sv);

	//---------------------------------------------
	std::string_view str(aStr.data(), readChars);

	if (isInt)
	{
		s_int_64 val = SToSI64(str);
		return {count_t::SMake(readChars), val};
	} else
	{
		double val = SToDouble_fast(str);
		return {count_t::SMake(readChars), val};
	}
}

count_t	GpStringOps::SFromBytes (const std::byte* _R_	aData,
								 const count_t			aDataSize,
								 char* _R_				aStrOut,
								 const count_t			aStrOutMaxSize)
{
	const size_t dataSize = aDataSize.Value();

	THROW_GPE_COND_CHECK_M(aStrOutMaxSize.Value() >= (dataSize*2), "'aStrOutMaxSize' size is too small"_sv);

	u_int_8* _R_		strPtr	= reinterpret_cast<u_int_8*>(aStrOut);
	const u_int_8* _R_	bytesPtr= reinterpret_cast<const u_int_8*>(aData);

	size_t byteId;

	for (byteId = 0; byteId < dataSize; ++byteId)
	{
		const u_int_8 b		= *bytesPtr++;
		const u_int_8 lo	= (b & 0x0F) >> 0;
		const u_int_8 hi	= (b & 0xF0) >> 4;

		*strPtr++ = u_int_8((hi < u_int_8(10)) ? (u_int_8('0')+ hi) : (u_int_8('a') + (hi - u_int_8(10))));
		*strPtr++ = u_int_8((lo < u_int_8(10)) ? (u_int_8('0')+ lo) : (u_int_8('a') + (lo - u_int_8(10))));
	}

	return count_t::SMake(byteId*2);
}

std::string	GpStringOps::SFromBytes (const GpBytesArray& aData)
{
	return SFromBytes(aData.data(), count_t::SMake(aData.size()));
}

std::string	GpStringOps::SFromBytes (const std::byte* _R_	aData,
									 const count_t			aDataSize)
{
	const size_t charsCount = aDataSize.Value() * 2;

	std::string res;
	res.resize(charsCount);

	if (SFromBytes(aData,
				   aDataSize,
				   res.data(),
				   count_t::SMake(charsCount)).Value() != charsCount)
	{
		THROW_GPE("Failed to convert bytes to hex string"_sv);
	}

	return res;
}

std::string	GpStringOps::SFromBytes (std::string_view aData)
{
	return SFromBytes(reinterpret_cast<const std::byte*>(aData.data()),
					  count_t::SMake(aData.size()));
}

count_t	GpStringOps::SToBytes (std::string_view	aStr,
							   std::byte*		aDataOut,
							   const count_t	aDataOutMaxSize)
{
	const char* _R_ strData	= aStr.data();
	size_t			strSize	= aStr.size();

	THROW_GPE_COND_CHECK_M(strSize > 0, "Empty string"_sv);
	THROW_GPE_COND_CHECK_M((strSize % 2) == 0, "String size must be even"_sv);
	THROW_GPE_COND_CHECK_M(aDataOutMaxSize.ValueAs<size_t>() >= (strSize/2), "aDataOutMaxSize is too small"_sv);

	std::string_view prefix = aStr.substr(0, 2);
	if ((prefix == "0x"_sv) ||
		(prefix == "0X"_sv))
	{
		strData += 2;
		strSize -= 2;
	}

	size_t			bytesCount	= 0;
	size_t			vl			= 0;
	u_int_8* _R_	bytesPtr	= reinterpret_cast<u_int_8*>(aDataOut);

	for (size_t	id = 0; id < strSize; ++id)
	{
		const char	ch	= *strData++;
		size_t		v	= 0;

		if ((ch >= '0') && (ch <= '9'))
		{
			v = size_t(ch) - size_t('0') + size_t(0);
		} else if ((ch >= 'a') && (ch <= 'f'))
		{
			v = size_t(ch) - size_t('a') + size_t(10);
		} else if ((ch >= 'A') && (ch <= 'F'))
		{
			v = size_t(ch) - size_t('A') + size_t(10);
		} else
		{
			THROW_GPE("Wrong character '"_sv + std::string_view(&ch, 1) + "' in hex sequence"_sv);
		}

		vl = (vl << 4) | v;

		if (bool(id & size_t(0b1)))
		{
			*bytesPtr++ = u_int_8(vl);
			vl = 0;
			bytesCount++;
		}
	}

	return count_t::SMake(bytesCount);
}

count_t	GpStringOps::SToBytes (std::string_view	aStr,
							   GpBytesArray&	aDataOut)
{
	size_t strSize = aStr.size();

	THROW_GPE_COND_CHECK_M(strSize > 0, "Empty string"_sv);
	THROW_GPE_COND_CHECK_M((strSize % 2) == 0, "String size must be even"_sv);

	std::string_view prefix = aStr.substr(0, 2);
	if ((prefix == "0x"_sv) ||
		(prefix == "0X"_sv))
	{
		aStr = std::string_view(aStr.data() + 2, strSize - 2);
		strSize = aStr.size();
	}

	const count_t bytesCount = count_t::SMake(strSize/2);

	const size_t oldCount = aDataOut.size();
	aDataOut.resize(oldCount + bytesCount.Value());

	if (SToBytes(aStr, aDataOut.data() + oldCount, bytesCount) != bytesCount)
	{
		THROW_GPE("Failed to convert hex string to bytes"_sv);
	}

	return bytesCount;
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
	u_int_16 utf16_val;
	std::memcpy(&utf16_val, aUTF16_value.data(), sizeof(u_int_16));
	utf16_val = BitOps::N2H(utf16_val);

	u_int_8* utf8_val = reinterpret_cast<u_int_8*>(aUTF8_valueOut.data());

	//Convert to UTF8
	if (u_int_64(utf16_val) < u_int_64(0x00000080))//0x00000000 — 0x0000007F
	{
		//0xxxxxxx
		*utf8_val++ = u_int_8(utf16_val);
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		return 1_cnt;
	} else if (u_int_64(utf16_val) < u_int_64(0x00000800))//0x00000080 — 0x000007FF
	{
		//110xxxxx 10xxxxxx
		*utf8_val++ = u_int_8(u_int_64(0xC0) + ((u_int_64(utf16_val) >> 6) & u_int_64(0x1F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf16_val) >> 0) & u_int_64(0x3F)));
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		return 2_cnt;
	} else
	{
		//1110xxxx 10xxxxxx 10xxxxxx
		*utf8_val++ = u_int_8(u_int_64(0xE0) + ((u_int_64(utf16_val) >> 12) & u_int_64(0x0F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf16_val) >> 6)  & u_int_64(0x3F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf16_val) >> 0)  & u_int_64(0x3F)));
		*utf8_val++ = 0;
		return 3_cnt;
	}
}

count_t		GpStringOps::SConv_UTF32_UTF8 (GpArray<std::byte, 4>&		aUTF8_valueOut,
										   const GpArray<std::byte, 4>	aUTF32_value)
{
	u_int_32 utf32_val;
	std::memcpy(&utf32_val, aUTF32_value.data(), sizeof(u_int_32));
	utf32_val = BitOps::N2H(utf32_val);

	u_int_8* utf8_val = reinterpret_cast<u_int_8*>(aUTF8_valueOut.data());

	//Convert to UTF8
	if (u_int_64(utf32_val) < u_int_64(0x00000080))//0x00000000 — 0x0000007F
	{
		//0xxxxxxx
		*utf8_val++ = u_int_8(utf32_val);
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		return 1_cnt;
	} else if (u_int_64(utf32_val) < u_int_64(0x00000800))//0x00000080 — 0x000007FF
	{
		//110xxxxx 10xxxxxx
		*utf8_val++ = u_int_8(u_int_64(0xC0) + ((u_int_64(utf32_val) >> 6) & u_int_64(0x1F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 0) & u_int_64(0x3F)));
		*utf8_val++ = 0;
		*utf8_val++ = 0;
		return 2_cnt;
	} else if (u_int_64(utf32_val) < u_int_64(0x00010000))//0x00000800 — 0x0000FFFF
	{
		//1110xxxx 10xxxxxx 10xxxxxx
		*utf8_val++ = u_int_8(u_int_64(0xE0) + ((u_int_64(utf32_val) >> 12) & u_int_64(0x0F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 6)  & u_int_64(0x3F)));
		*utf8_val++ = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 0)  & u_int_64(0x3F)));
		*utf8_val++ = 0;
		return 3_cnt;
	} else if (u_int_64(utf32_val) < u_int_64(0x00200000))//0x00010000 — 0x001FFFFF
	{
		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		utf8_val[0] = u_int_8(u_int_64(0xF0) + ((u_int_64(utf32_val) >> 18) & u_int_64(0x07)));
		utf8_val[1] = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 12) & u_int_64(0x3F)));
		utf8_val[2] = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 6)  & u_int_64(0x3F)));
		utf8_val[3] = u_int_8(u_int_64(0x80) + ((u_int_64(utf32_val) >> 0)  & u_int_64(0x3F)));
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
			data++;
		}
	}

	return aStr;
}

/*std::string_view	GpStringOps::SRemoveFromBegin (std::string_view aStr, const GpVector<char>& aChs)
{
	const size_t	size = aStr.size();
	const char* _R_ data = aStr.data();

	for (size_t id = 0; id < size; ++id)
	{
		const char ch1 = *data++;

		bool found = false;
		for (const char ch2: aChs)
		{
			if (ch1 == ch2)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return aStr.substr(id, size - id);
		}
	}

	return aStr;
}*/

/*std::string_view	GpStringOps::SRemoveFromEnd (std::string_view aStr, const GpVector<char>& aChs)
{
	const size_t	size = aStr.size();
	const char* _R_ data = aStr.data() + size - 1;

	for (size_t id = 0; id < size; ++id)
	{
		const char ch1 = *data--;

		bool found = false;
		for (const char ch2: aChs)
		{
			if (ch1 == ch2)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return aStr.substr(0, size - id);
		}
	}

	return aStr;
}*/

/*std::string_view	GpStringOps::SRemoveBeginEnd (std::string_view aStr, const GpVector<char>& aChs)
{
	return SRemoveFromEnd(SRemoveFromBegin(aStr, aChs), aChs);
}*/

void	GpStringOps::_SFromUI64 (const u_int_64	aValue,
								 char*			aStrOut,
								 const count_t	aOutLength)
{
	u_int_64		value	= aValue;
	const size_t	length	= aOutLength.ValueAs<size_t>();

	size_t next = length - 1;

	while (value >= u_int_64(100))
	{
		const size_t i = size_t(value % u_int_64(100)) << size_t(1);
		value /= u_int_64(100);
		aStrOut[next] = sDigits[i + 1];
		aStrOut[next - 1] = sDigits[i];
		next -= 2;
	}

	// Handle last 1-2 digits
	if (value < u_int_64(10))
	{
		aStrOut[next] = char(u_int_32('0') + u_int_32(value));
	} else
	{
		const size_t i = size_t(value) << size_t(1);
		aStrOut[next] = sDigits[i + 1];
		aStrOut[next - 1] = sDigits[i];
	}
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
