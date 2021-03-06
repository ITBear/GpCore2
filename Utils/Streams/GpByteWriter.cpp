#include "GpByteWriter.hpp"

namespace GPlatform {

GpByteWriter&   GpByteWriter::OffsetAdd (const size_byte_t aOffset)
{
    if (aOffset == 0_byte)
    {
        return *this;
    }

    if (iStorage.SizeLeft() < aOffset)
    {
        iStorage.AllocateNext(aOffset);
    }

    iStorage.OffsetAdd(aOffset);

    return *this;
}

GpByteWriter&   GpByteWriter::UInt8 (const u_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SInt8 (const s_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UInt16 (const u_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SInt16 (const s_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UInt32 (const u_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SInt32 (const s_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UInt64 (const u_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SInt64 (const s_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::BytesWithLen (GpRawPtrByteR aData)
{
    const u_int_32 size = aData.CountLeft().As<u_int_32>();
    CompactUInt32(size);

    if (size > 0)
    {
        Bytes(aData);
    }

    return *this;
}

GpByteWriter&   GpByteWriter::Bytes (GpRawPtrByteR aData)
{
    const size_byte_t dataSize = aData.SizeLeft();

    if (dataSize == 0_byte)
    {
        return *this;
    }

    if (iStorage.SizeLeft() < dataSize)
    {
        iStorage.AllocateNext(dataSize);
    }

    iStorage.WriteAndShift(aData);

    return *this;
}

GpByteWriter&   GpByteWriter::CompactUInt32 (const u_int_32 aValue)
{
    THROW_GPE_COND
    (
        (aValue <= u_int_32(0x0FFFFFFF)),
        "aValue is out of range"_sv
    );

    std::array <u_int_8, 4> buf;

    size_t      i       = buf.size();
    u_int_32    value   = aValue;

    do
    {
        buf.data()[--i] = u_int_8(value & u_int_32(0b01111111));
        value = value >> 7;
    } while (value > 0);

    while (i < buf.size())
    {
        if (i != buf.size() - 1)
        {
            UInt8(u_int_8(buf[i++]) | u_int_8(0b10000000));
        } else
        {
            UInt8(u_int_8(buf[i++]));
        }
    }

    return *this;
}

GpByteWriter&   GpByteWriter::CompactUInt16 (const u_int_16 aValue)
{
    THROW_GPE_COND
    (
        (aValue <= u_int_16(0x0FFF)),
        "aValue is out of range"_sv
    );

    std::array <u_int_8, 4> buf;

    size_t      i       = buf.size();
    u_int_16    value   = aValue;

    do
    {
        buf.data()[--i] = u_int_8(value & u_int_16(0b01111111));
        value = value >> 7;
    } while (value > 0);

    while (i < buf.size())
    {
        if (i != buf.size() - 1)
        {
            UInt8(u_int_8(buf[i++]) | u_int_8(0b10000000));
        } else
        {
            UInt8(u_int_8(buf[i++]));
        }
    }

    return *this;
}

}//GPlatform
