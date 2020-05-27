#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpBitIStream
{
	CLASS_REMOVE_CTRS(GpBitIStream);

public:
	inline				GpBitIStream	(const std::byte*	aData,
										 const size_bit_t	aCount,
										 const size_bit_t	aOffset) noexcept;
	inline				GpBitIStream	(std::string_view	aData) noexcept;
	inline				GpBitIStream	(std::string_view	aData,
										 const size_bit_t	aOffset) noexcept;
	inline				GpBitIStream	(const GpBytesArray&	aData) noexcept;
	inline				GpBitIStream	(const GpBytesArray&	aData,
										 const size_bit_t		aOffset) noexcept;
						~GpBitIStream	(void) noexcept = default;

	void				Bits			(std::byte*			aDataOut,
										 const size_bit_t	aCount);

	size_bit_t			LeftToRead		(void) const noexcept {return iLeftToRead;}

protected:
	const std::byte*	iData;
	const size_bit_t	iCount;
	const size_bit_t	iOffset;
	size_bit_t			iLeftToRead;
};

GpBitIStream::GpBitIStream (const std::byte*	aData,
							const size_bit_t	aCount,
							const size_bit_t	aOffset) noexcept:
iData(aData),
iCount(aCount),
iOffset(aOffset),
iLeftToRead(aCount)
{
}

GpBitIStream::GpBitIStream (std::string_view aData) noexcept:
GpBitIStream(reinterpret_cast<const std::byte*>(aData.data()),
			 size_byte_t::SMake(aData.size()),
			 0_bit)
{
}

GpBitIStream::GpBitIStream (std::string_view	aData,
							const size_bit_t	aOffset) noexcept:
GpBitIStream(reinterpret_cast<const std::byte*>(aData.data()),
			 size_byte_t::SMake(aData.size()),
			 aOffset)
{
}

GpBitIStream::GpBitIStream (const GpBytesArray&	aData) noexcept:
GpBitIStream(aData.data(),
			 size_byte_t::SMake(aData.size()),
			 0_bit)
{
}

GpBitIStream::GpBitIStream (const GpBytesArray&	aData,
							const size_bit_t	aOffset) noexcept:
GpBitIStream(aData.data(),
			 size_byte_t::SMake(aData.size()),
			 aOffset)
{
}

}//GPlatform
