#include "GpBitIStream.hpp"
#include "../../Exceptions/GpException.hpp"

namespace GPlatform {

void	GpBitIStream::Bits (std::byte*			aDataOut,
							const size_bit_t	aCount)
{
	THROW_GPE_COND_CHECK_M(   (aCount > 0_bit)
						   && (iLeftToRead >= aCount), "Out of range"_sv);

	size_t			leftToRead		= iLeftToRead.ValueAs<size_t>();
	const size_t	finalReadBits	= (iOffset + iCount).ValueAs<size_t>();

	size_t			leftToWrite		= aCount.ValueAs<size_t>();
	const size_t	finalWriteBits	= leftToWrite;

	const u_int_8*	dataIn			= reinterpret_cast<const u_int_8*>(iData);
	u_int_8*		dataOut			= reinterpret_cast<u_int_8*>(aDataOut);

	while (leftToWrite > 0)
	{
		const size_t readStartBitId		= finalReadBits - leftToRead;
		const size_t readByteID			= readStartBitId >> size_t(3);			// div 8
		const size_t readBitSH			= readStartBitId & size_t(0b00000111);	// mod 8
		const size_t canReadBits		= 8 - readBitSH;

		const size_t writeStartBitId	= finalWriteBits - leftToWrite;
		const size_t writeByteID		= writeStartBitId >> size_t(3);			// div 8
		const size_t writeBitSH			= writeStartBitId & size_t(0b00000111);	// mod 8
		const size_t canWriteBits		= 8 - writeBitSH;

		const size_t partSize			= std::min(std::min(canReadBits, canWriteBits), leftToWrite);

		const size_t readMask			= (size_t(1) << partSize) - size_t(1);
		const size_t readByte			= (size_t(dataIn[readByteID]) >> readBitSH) & readMask;

		const size_t writeMask			= (size_t(1) << writeBitSH) - size_t(1);
		const size_t writeByte			= size_t(dataOut[writeByteID]);

		dataOut[writeByteID]			= u_int_8((writeByte & writeMask) | (readByte << writeBitSH));

		leftToRead	-= partSize;
		leftToWrite -= partSize;
	}

	iLeftToRead -= aCount;
}

}//GPlatform
