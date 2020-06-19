#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_UUID)

#include "../Numerics/GpNumerics.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Strings/GpStringOps.hpp"

namespace GPlatform {

class GpRandom;

class GPCORE_API GpUUID
{
public:
	CLASS_DECLARE_DEFAULTS(GpUUID);

	using DataT	= GpArray<std::byte, 16>;

public:
	constexpr							GpUUID			(void) noexcept:iData(CE_Zero()) {}
	constexpr							GpUUID			(const DataT& aData) noexcept:iData(aData) {}
	constexpr							GpUUID			(const GpUUID& aUUID) noexcept:iData(aUUID.iData) {}
	constexpr							GpUUID			(GpUUID&& aUUID) noexcept:iData(std::move(aUUID.iData)) {}
										~GpUUID			(void) noexcept = default;

	[[nodiscard]] const DataT&			Data			(void) const noexcept {return iData;}
	[[nodiscard]] std::string_view		AsStringView	(void) const noexcept {return std::string_view(reinterpret_cast<const char*>(Data().data()), Data().size());}

	[[nodiscard]] std::string			ToString		(void) const;
	void								FromString		(std::string_view aStr);

	inline void							Set				(const GpUUID& aUUID) noexcept;
	inline void							Set				(GpUUID&& aUUID) noexcept;
	inline constexpr void				Set				(const DataT& aData) noexcept;
	inline constexpr void				Set				(DataT&& aData) noexcept;
	[[nodiscard]] inline bool			IsEqual			(const GpUUID& aUUID) const noexcept;
	[[nodiscard]] inline constexpr bool	IsEqual			(const DataT& aData) const noexcept;
	[[nodiscard]] inline constexpr bool	IsZero			(void) const noexcept;
	[[nodiscard]] inline constexpr bool	IsNotZero		(void) const noexcept;
	inline constexpr void				Zero			(void) noexcept;

	inline GpUUID&						operator=		(const GpUUID& aUUID) noexcept;
	inline GpUUID&						operator=		(GpUUID&& aUUID) noexcept;
	inline constexpr GpUUID&			operator=		(const DataT& aData) noexcept;
	inline constexpr GpUUID&			operator=		(DataT&& aData) noexcept;

	inline bool							operator>		(const GpUUID& aUUID) const noexcept;
	inline constexpr bool				operator>		(const DataT& aData) const noexcept;
	inline bool							operator<		(const GpUUID& aUUID) const noexcept;
	inline constexpr bool				operator<		(const DataT& aData) const noexcept;
	inline bool							operator==		(const GpUUID& aUUID) const noexcept;
	inline constexpr bool				operator==		(const DataT& aData) const noexcept;
	inline bool							operator!=		(const GpUUID& aUUID) const noexcept;
	inline constexpr bool				operator!=		(const DataT& aData) const noexcept;

	void								FromRandom		(GpRandom& aRandom);

	static GpUUID						SGenRandom		(void);
	static GpUUID						SFromString		(std::string_view aStr);

	[[nodiscard]] inline static constexpr DataT
										CE_FromString	(std::string_view aStr);
	[[nodiscard]] inline static constexpr DataT
										CE_Zero			(void);

private:
	DataT								iData;
};

void	GpUUID::Set (const GpUUID& aUUID) noexcept
{
	MemOps::SCopy(iData, aUUID.iData);
}

void	GpUUID::Set (GpUUID&& aUUID) noexcept
{
	MemOps::SConstructAndMove(iData, std::move(aUUID.iData));
}

constexpr void	GpUUID::Set (const DataT& aData) noexcept
{
	MemOps::SCopy(iData, aData);
}

constexpr void	GpUUID::Set (DataT&& aData) noexcept
{
	MemOps::SConstructAndMove(iData, std::move(aData));
}

bool	GpUUID::IsEqual (const GpUUID& aUUID) const noexcept
{
	return MemOps::SCompare(iData, aUUID.iData) == 0;
}

constexpr bool	GpUUID::IsEqual (const DataT& aData) const noexcept
{
	return MemOps::SCompare(iData, aData) == 0;
}

constexpr bool	GpUUID::IsZero (void) const noexcept
{
	return IsEqual(CE_Zero());
}

constexpr bool	GpUUID::IsNotZero (void) const noexcept
{
	return !IsZero();
}

constexpr void	GpUUID::Zero (void) noexcept
{
	MemOps::SCopy(iData, CE_Zero());
}

GpUUID&	GpUUID::operator= (const GpUUID& aUUID) noexcept
{
	Set(aUUID);
	return *this;
}

GpUUID&	GpUUID::operator= (GpUUID&& aUUID) noexcept
{
	Set(aUUID);
	return *this;
}

constexpr GpUUID&	GpUUID::operator= (const DataT& aData) noexcept
{
	Set(aData);
	return *this;
}

constexpr GpUUID&	GpUUID::operator= (DataT&& aData) noexcept
{
	Set(std::move(aData));
	return *this;
}

bool	GpUUID::operator> (const GpUUID& aUUID) const noexcept
{
	return MemOps::SCompare(iData, aUUID.iData) > 0;
}

constexpr bool	GpUUID::operator> (const DataT& aData) const noexcept
{
	return MemOps::SCompare(iData, aData) > 0;
}

bool	GpUUID::operator< (const GpUUID& aUUID) const noexcept
{
	return MemOps::SCompare(iData, aUUID.iData) < 0;
}

constexpr bool	GpUUID::operator< (const DataT& aData) const noexcept
{
	return MemOps::SCompare(iData, aData) < 0;
}

bool	GpUUID::operator== (const GpUUID& aUUID) const noexcept
{
	return MemOps::SCompare(iData, aUUID.iData) == 0;
}

constexpr bool	GpUUID::operator== (const DataT& aData) const noexcept
{
	return MemOps::SCompare(iData, aData) == 0;
}

bool	GpUUID::operator!= (const GpUUID& aUUID) const noexcept
{
	return MemOps::SCompare(iData, aUUID.iData) != 0;
}

constexpr bool	GpUUID::operator!= (const DataT& aData) const noexcept
{
	return MemOps::SCompare(iData, aData) != 0;
}

constexpr GpUUID::DataT	GpUUID::CE_FromString (std::string_view aStr)
{
	if (aStr.length() != 36)
	{
		THROW_GPE("Length of UUID string must be 36"_sv);
	}

	DataT data = {};

	const char* _R_	strPtr	= aStr.data();
	std::byte* _R_	dataPtr	= data.data();

	for (size_t id = 0; id < data.size(); ++id)
	{
		GpArray<char,2> str = {*strPtr++, *strPtr++};

		*dataPtr++ = GpStringOps::SToByte(str);

		if ((id == 3) ||
			(id == 5) ||
			(id == 7) ||
			(id == 9))
		{
			++strPtr;
		}
	}

	return data;

	/*const char*	strPtr	= aStr.data();
	DataT		uuid	= {};

	for (size_t id = 0; id < 16; id++)
	{
		if ((id == 4) ||
			(id == 6) ||
			(id == 8) ||
			(id == 10))
		{
			strPtr++;
		}

		GpArray<char,2> octetStr = {0, 0};
		octetStr.data()[0] = *strPtr++;
		octetStr.data()[1] = *strPtr++;

		uuid.data()[id] = std::byte(GpStringOps::SToByte(octetStr));
	}

	return uuid;*/
}

constexpr GpUUID::DataT	GpUUID::CE_Zero (void)
{
	return DataT {std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0),
				  std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0)};
}

}//GPlatform

//*******************************************
namespace std {

/*template<> struct hash<GPlatform::GpUUID>
{
	using argument_type = GPlatform::GpUUID;
	using result_type	= size_t;

	result_type operator()(argument_type const& aArg) const noexcept
	{
		if constexpr (sizeof(result_type) == sizeof(u_int_64))
		{
			const std::byte* data = aArg.Data();
			result_type part_0;
			result_type part_1;

			std::memcpy(&part_0, data + 0*sizeof(result_type), sizeof(result_type));
			std::memcpy(&part_1, data + 1*sizeof(result_type), sizeof(result_type));

			return std::hash<result_type>()(part_0) ^
				   std::hash<result_type>()(part_1);
		} else if constexpr (sizeof(result_type) == sizeof(u_int_32))
		{
			const std::byte* data = aArg.Data();
			result_type part_0;
			result_type part_1;
			result_type part_2;
			result_type part_3;

			std::memcpy(&part_0, data + 0*sizeof(result_type), sizeof(result_type));
			std::memcpy(&part_1, data + 1*sizeof(result_type), sizeof(result_type));
			std::memcpy(&part_2, data + 2*sizeof(result_type), sizeof(result_type));
			std::memcpy(&part_3, data + 3*sizeof(result_type), sizeof(result_type));

			return std::hash<result_type>()(part_0) ^
				   std::hash<result_type>()(part_1) ^
				   std::hash<result_type>()(part_2) ^
				   std::hash<result_type>()(part_3);
		}
	}
};*/

/*template<> struct hash<std::pair<GPlatform::GpUUID, GPlatform::GpUUID>>
{
	using argument_type = std::pair<GPlatform::GpUUID, GPlatform::GpUUID>;
	using result_type	= size_t;

	result_type operator()(argument_type const& aArg) const noexcept
	{
		return hash<GPlatform::GpUUID>()(aArg.first) ^
			   hash<GPlatform::GpUUID>()(aArg.second);
	}
};*/

/*template<> struct equal_to<GPlatform::GpUUID>
{
	using argument_type = GPlatform::GpUUID;

	bool	operator()(const argument_type& aA,
					   const argument_type& aB) const noexcept
	{
		return aA == aB;
	}
};*/

inline string to_string(const GPlatform::GpUUID& aUUID)
{
	return aUUID.ToString();
}

}//std

#endif//#if defined(GP_USE_UUID)
