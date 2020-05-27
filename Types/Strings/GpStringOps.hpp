#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "GpStringLiterals.hpp"
#include "GpStringOpsGlob.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Containers/GpBytesArray.hpp"
#include "../Units/Other/count_t.hpp"
#include "../../Algorithms/GpSplit.hpp"

#include <variant>

namespace GPlatform {

class GPCORE_API GpStringOps
{
	CLASS_REMOVE_CTRS(GpStringOps);

public:
	static GpVector<std::string>			SSplitToStr				(std::string_view		aSourceStr,
																	 const char				aDelim,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	static GpVector<std::string>			SSplitToStr				(std::string_view		aSourceStr,
																	 std::string_view		aDelim,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	static GpVector<std::string>			SSplitToStr				(std::string_view		aSourceStr,
																	 const char				aDelim,
																	 const char				aEscGroupBegin,
																	 const char				aEscGroupEnd,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	static GpVector<std::string_view>		SSplitToView			(std::string_view		aSourceStr,
																	 const char				aDelim,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	static GpVector<std::string_view>		SSplitToView			(std::string_view		aSourceStr,
																	 std::string_view		aDelim,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	static GpVector<std::string_view>		SSplitToView			(std::string_view		aSourceStr,
																	 const char				aDelim,
																	 const char				aEscGroupBegin,
																	 const char				aEscGroupEnd,
																	 const count_t			aReturnPartsCountLimit,
																	 const count_t			aDelimCountLimit,
																	 const Algo::SplitMode	aSplitMode);

	//------------------------- Numeric from/to string --------------------------
	static count_t							SFromUI64				(const u_int_64	aValue,
																	 char*			aStrOut,
																	 const count_t	aMaxOutLength);
	static std::string						SFromUI64				(const u_int_64	aValue);

	static count_t							SFromSI64				(const s_int_64	aValue,
																	 char*			aStrOut,
																	 const count_t	aMaxOutLength);
	static std::string						SFromSI64				(const s_int_64	aValue);

	static count_t							SFromDouble				(const double	aValue,
																	 char*			aStrOut,
																	 const count_t	aMaxOutLength);
	static std::string						SFromDouble				(const double aValue);

	static u_int_64							SToUI64					(std::string_view aStr);
	static s_int_64							SToSI64					(std::string_view aStr);
	static double							SToDouble_fast			(std::string_view aStr);//Supported format: [+-][u_int_64][.[u_int_64]]

	//[+-][u_int_64][.[u_int_64]] - DOUBLE
	//[+-]digits - INT
	static GpTuple<count_t, std::variant<s_int_64, double>>
											SToNumeric				(std::string_view aStr);

	//------------------------- Bytes from/to string --------------------------
	static count_t							SFromBytes				(const std::byte* _R_	aData,
																	 const count_t			aDataSize,
																	 char*					aStrOut,
																	 const count_t			aStrOutMaxSize);
	static std::string						SFromBytes				(const GpBytesArray&	aData);
	static std::string						SFromBytes				(const std::byte* _R_	aData,
																	 const count_t			aDataSize);
	static std::string						SFromBytes				(std::string_view		aData);
	static inline GpArray<char,2>			SFromByte				(const std::byte		aData) noexcept;

	static count_t							SToBytes				(std::string_view	aStr,
																	 std::byte*			aDataOut,
																	 const count_t		aDataOutMaxSize);
	static count_t							SToBytes				(std::string_view	aStr,
																	 GpBytesArray&		aDataOut);
	static GpBytesArray						SToBytes				(std::string_view	aStr);

	static constexpr std::byte				SToByte					(GpArray<char,2>	aStr);

	//------------------------- Unicode --------------------------
	static count_t							SConv_UTF16_UTF8		(GpArray<std::byte, 4>&			aUTF8_valueOut,
																	 const GpArray<std::byte, 2>	aUTF16_value);//return bytes count of aUTF8Out
	static count_t							SConv_UTF32_UTF8		(GpArray<std::byte, 4>&			aUTF8_valueOut,
																	 const GpArray<std::byte, 4>	aUTF32_value);//return bytes count of aUTF8Out

	//------------------------- Remove --------------------------
	//static std::string_view				SRemoveFromBegin		(std::string_view aStr, const GpVector<char>& aChs);
	//static std::string_view				SRemoveFromEnd			(std::string_view aStr, const GpVector<char>& aChs);
	//static std::string_view				SRemoveBeginEnd			(std::string_view aStr, const GpVector<char>& aChs);

	//------------------------- Replace --------------------------
	static std::string&						SReplace				(std::string& aStr, const char aChar, const char aNewChar);

	//------------------------- Compare -----------------------------
	static inline constexpr bool			SIsStartWith			(std::string_view aStr, std::string_view aStartStr) noexcept;
	static inline constexpr bool			SIsEndWith				(std::string_view aStr, std::string_view aEndStr) noexcept;

	//------------------------- Count -----------------------------
	static inline constexpr count_t			SCountChars				(std::string_view aStr, const char aChar) noexcept;
	static inline constexpr count_t			SCountSubstr			(std::string_view aStr, std::string_view aSubStr) noexcept;

private:
	static void								_SFromUI64				(const u_int_64	aValue,
																	 char*			aStrOut,
																	 const count_t	aOutLength);

private:
	static const GpArray<char, 201>			sDigits;
};


GpArray<char,2>	GpStringOps::SFromByte (const std::byte aData) noexcept
{
	GpArray<char,2> res = {0, 0};

	const u_int_8 lo = (u_int_8(aData) & 0x0F) >> 0;
	const u_int_8 hi = (u_int_8(aData) & 0xF0) >> 4;

	res.at(0) = char((size_t(hi) < size_t(10)) ? (size_t('0') + size_t(hi)) : (size_t('a') + (size_t(hi) - size_t(10))));
	res.at(1) = char((size_t(lo) < size_t(10)) ? (size_t('0') + size_t(lo)) : (size_t('a') + (size_t(lo) - size_t(10))));

	return res;
}

constexpr std::byte	GpStringOps::SToByte (GpArray<char,2> aStr)
{
	//--------------------------
	char	ch		= aStr.data()[0];
	u_int_8	valHi	= 0;
	u_int_8	valLo	= 0;
	u_int_8	beginCh	= 0;
	u_int_8	shift	= 0;

	if ((ch >= '0') && (ch <= '9'))		{beginCh = u_int_8('0'); shift = 0;}
	else if ((ch >= 'A') && (ch <= 'Z')){beginCh = u_int_8('A'); shift = 10;}
	else if ((ch >= 'a') && (ch <= 'z')){beginCh = u_int_8('a'); shift = 10;}
	else GpThrowCe<std::out_of_range>("Wrong HEX character");

	valHi = u_int_8(u_int_8(ch) - beginCh + shift);

	//--------------------------
	ch = aStr.data()[1];

	if ((ch >= '0') && (ch <= '9'))		{beginCh = u_int_8('0'); shift = 0;}
	else if ((ch >= 'A') && (ch <= 'Z')){beginCh = u_int_8('A'); shift = 10;}
	else if ((ch >= 'a') && (ch <= 'z')){beginCh = u_int_8('a'); shift = 10;}
	else GpThrowCe<std::out_of_range>("Wrong HEX character");

	valLo = u_int_8(u_int_8(ch) - beginCh + shift);

	return std::byte(u_int_8(valHi << 4) | u_int_8(valLo));
}

constexpr bool	GpStringOps::SIsStartWith (std::string_view aStr, std::string_view aStartStr) noexcept
{
	return aStr.starts_with(aStartStr);
}

constexpr bool	GpStringOps::SIsEndWith (std::string_view aStr, std::string_view aEndStr) noexcept
{
	return aStr.ends_with(aEndStr);
}

constexpr count_t	GpStringOps::SCountChars (std::string_view aStr, const char aChar) noexcept
{
	size_t		count	= 0;
	const char*	data	= aStr.data();
	for (size_t id = 0; id < aStr.size(); ++id)
	{
		if (*data++ == aChar)
		{
			count++;
		}
	}

	return count_t::SMake(count);
}

constexpr count_t	GpStringOps::SCountSubstr (std::string_view aStr, std::string_view aSubStr) noexcept
{
	const size_t				subStrLength	= aSubStr.length();
	size_t						count			= 0;
	std::string_view::size_type	startId			= 0;

	while ((startId = aStr.find(aSubStr, startId)) != std::string_view::npos)
	{
		startId += subStrLength;
		count++;
	}

	return count_t::SMake(count);
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
