#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

template<typename StorageT>
class GPCORE_API GpBitOStream
{
	CLASS_REMOVE_CTRS(GpBitOStream);

public:
						GpBitOStream	(std::byte*			aData,
										 const size_bit_t	aCount,
										 const size_bit_t	aOffset) noexcept;
						~GpBitOStream	(void) noexcept = default;

	void				Bits			(const std::byte*	aData,
										 const size_bit_t	aCount);

	size_bit_t			LeftToWite		(void) const noexcept {return iLeftToWrite;}

private:
	void				AllocateNext	(const size_bit_t aSize);

protected:
	std::byte*			iData;
	size_bit_t			iCount;
	const size_bit_t	iOffset;
	size_bit_t			iLeftToWrite;
};

template<typename StorageT>
GpBitOStream<StorageT>::GpBitOStream (std::byte*		aData,
									  const size_bit_t	aCount,
									  const size_bit_t	aOffset) noexcept:
iData(aData),
iCount(aCount),
iOffset(aOffset),
iLeftToWrite(aCount)
{
}

template<typename StorageT>
void	GpBitOStream<StorageT>::Bits (const std::byte*	aData,
									  const size_bit_t	aCount)
{
	THROW_GPE_COND_CHECK_M(aCount > 0_bit, "aCount is 0"_sv);

	AllocateNext(aCount);

	size_t			leftToRead		= aCount.ValueAs<size_t>();
	const size_t	finalReadBits	= leftToRead;

	size_t			leftToWrite		= iLeftToWrite.ValueAs<size_t>();
	const size_t	finalWriteBits	= (iOffset + iCount).template ValueAs<size_t>();

	const u_int_8*	dataIn			= reinterpret_cast<const u_int_8*>(aData);
	u_int_8*		dataOut			= reinterpret_cast<u_int_8*>(iData);

	while (leftToRead > 0)
	{
		const size_t readStartBitId		= finalReadBits - leftToRead;
		const size_t readByteID			= readStartBitId >> size_t(3);			// div 8
		const size_t readBitSH			= readStartBitId & size_t(0b00000111);	// mod 8
		const size_t canReadBits		= 8 - readBitSH;

		const size_t writeStartBitId	= finalWriteBits - leftToWrite;
		const size_t writeByteID		= writeStartBitId >> size_t(3);			// div 8
		const size_t writeBitSH			= writeStartBitId & size_t(0b00000111);	// mod 8
		const size_t canWriteBits		= 8 - writeBitSH;

		const size_t partSize			= std::min(std::min(canReadBits, canWriteBits), leftToRead);

		const size_t readMask			= (size_t(1) << partSize) - size_t(1);
		const size_t readByte			= (size_t(dataIn[readByteID]) >> readBitSH) & readMask;

		const size_t writeMask			= (size_t(1) << writeBitSH) - size_t(1);
		const size_t writeByte			= size_t(dataOut[writeByteID]);

		dataOut[writeByteID]			= u_int_8((writeByte & writeMask) | (readByte << writeBitSH));

		leftToRead	-= partSize;
		leftToWrite -= partSize;
	}

	iLeftToWrite -= aCount;
}

template<typename StorageT>
void	GpBitOStream<StorageT>::AllocateNext (const size_bit_t aSize)
{
	static_cast<StorageT*>(this)->_AllocateNext(aSize);
}

}//GPlatform
