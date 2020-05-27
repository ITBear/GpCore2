#pragma once

#include "GpRandom.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

namespace GPlatform {

class GPCORE_API GpSRandom
{
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpSRandom);

private:
								GpSRandom		(void) noexcept;
								~GpSRandom		(void) noexcept;

public:
	static GpSRandom&			S				(void) noexcept;

	GpSpinlock&					Lock			(void) noexcept {return iLock;}

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
	GpRandom					iRandom;
	mutable GpSpinlock			iLock;
};

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
