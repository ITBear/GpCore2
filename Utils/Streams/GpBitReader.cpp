#include "GpBitReader.hpp"
#include "../../Exceptions/GpException.hpp"

namespace GPlatform {

void	GpBitReader::Bits (std::byte*		aDataOut,
						   const size_bit_t	aSize)
{
	const size_bit_t left = iStorage.Left();

	THROW_GPE_COND_CHECK_M(   (aSize > 0_bit)
						   && (left >= aSize), "Out of range"_sv);

	size_t			leftToRead		= left.ValueAs<size_t>();
	const size_t	finalReadBits	= (iStorage.Offset() + iStorage.Size()).ValueAs<size_t>();

	size_t			leftToWrite		= aSize.ValueAs<size_t>();
	const size_t	finalWriteBits	= leftToWrite;

	const u_int_8*	dataIn			= reinterpret_cast<const u_int_8*>(iStorage.Data());
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

	iStorage.SetLeftSub(aSize);
}

u_int_8	GpBitReader::UInt8 (void)
{
	return ReadPOD<u_int_8>(size_byte_t::SMake(sizeof(u_int_8)));
}

u_int_8	GpBitReader::UInt8 (const size_bit_t aSize)
{
	return ReadPOD<u_int_8>(aSize);
}

s_int_8	GpBitReader::SInt8 (void)
{
	return ReadPOD<s_int_8>(size_byte_t::SMake(sizeof(s_int_8)));
}

s_int_8	GpBitReader::SInt8 (const size_bit_t aSize)
{
	return ReadPOD<s_int_8>(aSize);
}

u_int_16	GpBitReader::UInt16 (void)
{
	return ReadPOD<u_int_16>(size_byte_t::SMake(sizeof(u_int_16)));
}

u_int_16	GpBitReader::UInt16 (const size_bit_t aSize)
{
	return ReadPOD<u_int_16>(aSize);
}

s_int_16	GpBitReader::SInt16 (void)
{
	return ReadPOD<s_int_16>(size_byte_t::SMake(sizeof(s_int_16)));
}

s_int_16	GpBitReader::SInt16 (const size_bit_t aSize)
{
	return ReadPOD<s_int_16>(aSize);
}

u_int_32	GpBitReader::UInt32 (void)
{
	return ReadPOD<u_int_32>(size_byte_t::SMake(sizeof(u_int_32)));
}

u_int_32	GpBitReader::UInt32 (const size_bit_t aSize)
{
	return ReadPOD<u_int_32>(aSize);
}

s_int_32	GpBitReader::SInt32 (void)
{
	return ReadPOD<s_int_32>(size_byte_t::SMake(sizeof(s_int_32)));
}

s_int_32	GpBitReader::SInt32 (const size_bit_t aSize)
{
	return ReadPOD<s_int_32>(aSize);
}

u_int_64	GpBitReader::UInt64 (void)
{
	return ReadPOD<u_int_64>(size_byte_t::SMake(sizeof(u_int_64)));
}

u_int_64	GpBitReader::UInt64 (const size_bit_t aSize)
{
	return ReadPOD<u_int_64>(aSize);
}

s_int_64	GpBitReader::SInt64 (void)
{
	return ReadPOD<s_int_64>(size_byte_t::SMake(sizeof(s_int_64)));
}

s_int_64	GpBitReader::SInt64 (const size_bit_t aSize)
{
	return ReadPOD<s_int_64>(aSize);
}

size_t	GpBitReader::SizeT (const size_bit_t aSize)
{
	return ReadPOD<size_t>(aSize);
}

}//GPlatform
