#include "GpBitWriter.hpp"

namespace GPlatform {

void    GpBitWriter::Bits (const std::byte* aData,
                           const size_bit_t aSize)
{
    THROW_GPE_COND_CHECK_M(aSize > 0_bit, "aSize is 0"_sv);

    size_bit_t left = iStorage.Left();

    if (left < aSize)
    {
        iStorage.AllocateNext(aSize);
        left = iStorage.Left();
    }

    size_t          leftToRead      = aSize.ValueAs<size_t>();
    const size_t    finalReadBits   = leftToRead;

    size_t          leftToWrite     = left.ValueAs<size_t>();
    const size_t    finalWriteBits  = (iStorage.Offset() + iStorage.Size()).template ValueAs<size_t>();

    const u_int_8*  dataIn          = reinterpret_cast<const u_int_8*>(aData);
    u_int_8*        dataOut         = reinterpret_cast<u_int_8*>(iStorage.Data());

    while (leftToRead > 0)
    {
        const size_t readStartBitId     = finalReadBits - leftToRead;
        const size_t readByteID         = readStartBitId >> size_t(3);          // div 8
        const size_t readBitSH          = readStartBitId & size_t(0b00000111);  // mod 8
        const size_t canReadBits        = 8 - readBitSH;

        const size_t writeStartBitId    = finalWriteBits - leftToWrite;
        const size_t writeByteID        = writeStartBitId >> size_t(3);         // div 8
        const size_t writeBitSH         = writeStartBitId & size_t(0b00000111); // mod 8
        const size_t canWriteBits       = 8 - writeBitSH;

        const size_t partSize           = std::min(std::min(canReadBits, canWriteBits), leftToRead);

        const size_t readMask           = (size_t(1) << partSize) - size_t(1);
        const size_t readByte           = (size_t(dataIn[readByteID]) >> readBitSH) & readMask;

        const size_t writeMask          = (size_t(1) << writeBitSH) - size_t(1);
        const size_t writeByte          = size_t(dataOut[writeByteID]);

        dataOut[writeByteID]            = u_int_8((writeByte & writeMask) | (readByte << writeBitSH));

        leftToRead  -= partSize;
        leftToWrite -= partSize;
    }

    iStorage.SetLeftSub(aSize);
}


void GpBitWriter::UInt8 (const u_int_8 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(u_int_8)));
}

void GpBitWriter::UInt8 (const u_int_8 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::SInt8 (const s_int_8 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(s_int_8)));
}

void GpBitWriter::SInt8 (const s_int_8 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::UInt16 (const u_int_16 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(u_int_16)));
}

void GpBitWriter::UInt16 (const u_int_16 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::SInt16 (const s_int_16 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(s_int_16)));
}

void GpBitWriter::SInt16 (const s_int_16 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::UInt32 (const u_int_32 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(u_int_32)));
}

void GpBitWriter::UInt32 (const u_int_32 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::SInt32 (const s_int_32 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(s_int_32)));
}

void GpBitWriter::SInt32 (const s_int_32 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::UInt64 (const u_int_64 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(u_int_64)));
}

void GpBitWriter::UInt64 (const u_int_64 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::SInt64 (const s_int_64 aValue)
{
    WritePOD(aValue, size_byte_t::SMake(sizeof(s_int_64)));
}

void GpBitWriter::SInt64 (const s_int_64 aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

void GpBitWriter::SizeT (const size_t aValue, const size_bit_t aSize)
{
    WritePOD(aValue, aSize);
}

}//GPlatform
