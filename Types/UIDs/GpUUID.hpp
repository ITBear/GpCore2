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

	using RawT	= GpArray<std::byte, 16>;

public:
									GpUUID			(void) noexcept {std::memset(iRawData.data(), 0, sizeof(RawT));}
	constexpr						GpUUID			(const RawT& aRawData) noexcept:iRawData(aRawData) {}
									GpUUID			(const GpUUID& aUUID) noexcept:iRawData(aUUID.iRawData) {}
									GpUUID			(GpUUID&& aUUID) noexcept:iRawData(std::move(aUUID.iRawData)) {}
									~GpUUID			(void) noexcept = default;

	[[nodiscard]] RawT				RawData			(void) const noexcept {return iRawData;}
	[[nodiscard]] const std::byte*	Data			(void) const noexcept {return iRawData.data();}
	[[nodiscard]] std::string_view	DataAsSV		(void) const noexcept {return std::string_view(reinterpret_cast<const char*>(Data()), 16);}
	[[nodiscard]] std::string		ToString		(void) const;

	void							FromString		(std::string_view aStr);
	inline void						Set				(const GpUUID& aUUID) noexcept;
	[[nodiscard]] bool				IsEqual			(const GpUUID& aUUID) const noexcept;
	[[nodiscard]] bool				IsZero			(void) const noexcept;
	[[nodiscard]] inline bool		IsNotZero		(void) const noexcept;
	void							Zero			(void) noexcept;

	inline GpUUID&					operator=		(const GpUUID& aUUID) noexcept;
	inline GpUUID&					operator=		(GpUUID&& aUUID) noexcept;
	inline bool						operator>		(const GpUUID& aUUID) const noexcept;
	inline bool						operator<		(const GpUUID& aUUID) const noexcept;
	inline bool						operator==		(const GpUUID& aUUID) const noexcept;
	inline bool						operator!=		(const GpUUID& aUUID) const noexcept;

	void							SetRandom		(GpRandom& aRandom);

	[[nodiscard]] static GpUUID		SGenRandom		(void);
	[[nodiscard]] static GpUUID		SFromString		(std::string_view aStr);

	[[nodiscard]] inline static constexpr RawT
									CE_FromString	(std::string_view aStr);

private:
	RawT							iRawData;
};

void	GpUUID::Set (const GpUUID& aUUID) noexcept
{
	std::memcpy(iRawData.data(), aUUID.iRawData.data(), sizeof(RawT));
}

bool	GpUUID::IsNotZero (void) const noexcept
{
	return !IsZero();
}

GpUUID&	GpUUID::operator= (const GpUUID& aUUID) noexcept
{
	Set(aUUID); return *this;
}

GpUUID&	GpUUID::operator= (GpUUID&& aUUID) noexcept
{
	Set(aUUID); return *this;
}

bool	GpUUID::operator> (const GpUUID& aUUID) const noexcept
{
	return std::memcmp(iRawData.data(), aUUID.iRawData.data(), sizeof(RawT)) > 0;
}

bool	GpUUID::operator< (const GpUUID& aUUID) const noexcept
{
	return std::memcmp(iRawData.data(), aUUID.iRawData.data(), sizeof(RawT)) < 0;
}

bool	GpUUID::operator== (const GpUUID& aUUID) const noexcept
{
	return std::memcmp(iRawData.data(), aUUID.iRawData.data(), sizeof(RawT)) == 0;
}

bool	GpUUID::operator!= (const GpUUID& aUUID) const noexcept
{
	return std::memcmp(iRawData.data(), aUUID.iRawData.data(), sizeof(RawT)) != 0;
}

constexpr GpUUID::RawT	GpUUID::CE_FromString (std::string_view aStr)
{
	if (aStr.length() != 36)
	{
		GpThrowCe<std::out_of_range>("Length of UUID string must be 36");
	}

	const char*	strPtr	= aStr.data();
	RawT		uuid	= {};

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

	return uuid;
}

}//GPlatform

//********************** Hash *********************
namespace std {

template<> struct hash<GPlatform::GpUUID>
{
	using argument_type = GPlatform::GpUUID;
	using result_type	= size_t;

	result_type operator()(argument_type const& aArg) const noexcept
	{
		if constexpr (sizeof(size_t) == sizeof(GPlatform::u_int_64))
		{
			const std::byte* _R_ data = aArg.Data();
			size_t part_0;
			size_t part_1;

			std::memcpy(&part_0, data + 0*sizeof(size_t), sizeof(size_t));
			std::memcpy(&part_1, data + 1*sizeof(size_t), sizeof(size_t));

			return std::hash<size_t>()(part_0) ^
				   std::hash<size_t>()(part_1);
		} else
		{
			const std::byte* _R_ data = aArg.Data();
			size_t part_0;
			size_t part_1;
			size_t part_2;
			size_t part_3;

			std::memcpy(&part_0, data + 0*sizeof(size_t), sizeof(size_t));
			std::memcpy(&part_1, data + 1*sizeof(size_t), sizeof(size_t));
			std::memcpy(&part_2, data + 2*sizeof(size_t), sizeof(size_t));
			std::memcpy(&part_3, data + 3*sizeof(size_t), sizeof(size_t));

			return std::hash<size_t>()(part_0) ^
				   std::hash<size_t>()(part_1) ^
				   std::hash<size_t>()(part_2) ^
				   std::hash<size_t>()(part_3);
		}
	}
};

template<> struct hash<std::pair<GPlatform::GpUUID, GPlatform::GpUUID>>
{
	using argument_type = std::pair<GPlatform::GpUUID, GPlatform::GpUUID>;
	using result_type	= size_t;

	result_type operator()(argument_type const& aArg) const noexcept
	{
		return hash<GPlatform::GpUUID>()(aArg.first) ^
			   hash<GPlatform::GpUUID>()(aArg.second);
	}
};

template<> struct equal_to<GPlatform::GpUUID>
{
	using argument_type = GPlatform::GpUUID;

	bool	operator()(const argument_type& aA,
					   const argument_type& aB) const noexcept
	{
		return aA == aB;
	}
};

inline string to_string(const GPlatform::GpUUID& aUUID)
{
	return aUUID.ToString();
}

}//std

#endif//#if defined(GP_USE_UUID)
