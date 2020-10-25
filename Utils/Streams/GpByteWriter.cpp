#include "GpByteWriter.hpp"

namespace GPlatform {

void    GpByteWriter::UInt8 (const u_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::SInt8 (const s_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::UInt16 (const u_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::SInt16 (const s_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::UInt32 (const u_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::SInt32 (const s_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::UInt64 (const u_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::SInt64 (const s_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void    GpByteWriter::BytesWithLen (GpRawPtrByteR aData)
{
    const s_int_32 size = aData.CountLeft().As<s_int_32>();
    CompactSInt32(size);

    if (size > 0)
    {
        Bytes(aData);
    }
}

void    GpByteWriter::Bytes (GpRawPtrByteR aData)
{
    const size_byte_t dataSize = aData.SizeLeft();

    if (dataSize == 0_byte)
    {
        return;
    }

    if (iStorage.SizeLeft() < dataSize)
    {
        iStorage.AllocateNext(dataSize);
    }

    iStorage.WriteAndShift(aData);
}

void    GpByteWriter::CompactSInt32 (const s_int_32 aValue)
{
    THROW_GPE_COND_CHECK_M((aValue >= s_int_32(0)) && (aValue <= s_int_32(0x0FFFFFFF)), "aValue is out of range");

    std::array <u_int_32, 4> buf;

    size_t i = buf.size();
    s_int_32 value = aValue;
    do
    {
        buf.data()[--i] = value & 0b01111111;
        value = value >> 7;
    } while (value > 0);

    while (i < buf.size())
    {
        if (i != buf.size() - 1)
        {
            UInt8(u_int_8(buf[i++] | 0b10000000));
        } else
        {
            UInt8(u_int_8(buf[i++]));
        }
    }
}

}//GPlatform
