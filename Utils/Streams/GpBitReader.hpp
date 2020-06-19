#pragma once

#include "GpBitReaderStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitReader
{
	CLASS_REMOVE_CTRS(GpBitReader);

public:
							GpBitReader		(GpBitReaderStorage& aStorage) noexcept: iStorage(aStorage){}
							~GpBitReader	(void) noexcept = default;

	void					Bits			(std::byte*			aDataOut,
											 const size_bit_t	aSize);

	u_int_8					UInt8			(void);
	u_int_8					UInt8			(const size_bit_t aSize);
	s_int_8					SInt8			(void);
	s_int_8					SInt8			(const size_bit_t aSize);
	u_int_16				UInt16			(void);
	u_int_16				UInt16			(const size_bit_t aSize);
	s_int_16				SInt16			(void);
	s_int_16				SInt16			(const size_bit_t aSize);
	u_int_32				UInt32			(void);
	u_int_32				UInt32			(const size_bit_t aSize);
	s_int_32				SInt32			(void);
	s_int_32				SInt32			(const size_bit_t aSize);
	u_int_64				UInt64			(void);
	u_int_64				UInt64			(const size_bit_t aSize);
	s_int_64				SInt64			(void);
	s_int_64				SInt64			(const size_bit_t aSize);
	size_t					SizeT			(const size_bit_t aSize);

	size_bit_t				LeftToRead		(void) const noexcept {return iStorage.Left();}

private:
	template<typename T>
	T						ReadPOD			(const size_bit_t aSize)
	{
		THROW_GPE_COND_CHECK_M(aSize <= size_byte_t::SMake(sizeof(T)), "Out of range"_sv);

		T val;
		Bits(reinterpret_cast<std::byte*>(&val), aSize);

#if defined(GP_ORDER_BIG_ENDIAN)
		val = BitOps::BSwap(val);
#endif
		return val;
	}

private:
	GpBitReaderStorage&		iStorage;
};

}//GPlatform
