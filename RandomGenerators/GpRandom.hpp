#pragma once

#include "../GpCore_global.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../Multithreading/SyncPrimitives/GpSyncPrimitives.hpp"
#include "../Types/Numerics/GpNumericOps.hpp"
#include <random>
#include <array>
#include <type_traits>

namespace GPlatform {

#if defined(GP_ENVIRONMENT_64)
	using random_mt19937 = std::mt19937_64;
#elif defined(GP_ENVIRONMENT_32)
	using random_mt19937 = std::mt19937;
#else
#	error Unknown environment
#endif

class GPCORE_API GpRandom
{
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpRandom);

public:
								GpRandom		(void) noexcept;
								~GpRandom		(void) noexcept;

	void						SetSeedFromRD	(void);
	void						SetSeed			(const random_mt19937::result_type aSeed);

	[[nodiscard]] s_int_8		SI8				(const s_int_8	aMin = NumOps::SMin<s_int_8>(),
												 const s_int_8	aMax = NumOps::SMax<s_int_8>());
	[[nodiscard]] u_int_8		UI8				(const u_int_8	aMin = NumOps::SMin<u_int_8>(),
												 const u_int_8	aMax = NumOps::SMax<u_int_8>());
	[[nodiscard]] s_int_16		SI16			(const s_int_16	aMin = NumOps::SMin<s_int_16>(),
												 const s_int_16	aMax = NumOps::SMax<s_int_16>());
	[[nodiscard]] u_int_16		UI16			(const u_int_16	aMin = NumOps::SMin<u_int_16>(),
												 const u_int_16	aMax = NumOps::SMax<u_int_16>());
	[[nodiscard]] s_int_32		SI32			(const s_int_32	aMin = NumOps::SMin<s_int_32>(),
												 const s_int_32	aMax = NumOps::SMax<s_int_32>());
	[[nodiscard]] u_int_32		UI32			(const u_int_32	aMin = NumOps::SMin<u_int_32>(),
												 const u_int_32	aMax = NumOps::SMax<u_int_32>());
	[[nodiscard]] s_int_64		SI64			(const s_int_64	aMin = NumOps::SMin<s_int_64>(),
												 const s_int_64	aMax = NumOps::SMax<s_int_64>());
	[[nodiscard]] u_int_64		UI64			(const u_int_64	aMin = NumOps::SMin<u_int_64>(),
												 const u_int_64	aMax = NumOps::SMax<u_int_64>());

private:
	template<typename T,
			 template<typename> typename Distribution = std::uniform_int_distribution>
	[[nodiscard]] T				Next			(const T aMin = std::numeric_limits<T>::min(),
												 const T aMax = std::numeric_limits<T>::max())
	{
		static_assert(std::is_integral<T>(), "T must be integral type");

		Distribution<T> dist(aMin, aMax);
		return dist(iEngine);
	}

private:
	random_mt19937		iEngine;
};

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
