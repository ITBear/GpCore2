#include "GpByteReader.hpp"

namespace GPlatform {

u_int_8 GpByteReader::GpByteReader::UInt8 (void)
{
    return ReadPOD<u_int_8>();
}

s_int_8 GpByteReader::GpByteReader::SInt8 (void)
{
    return ReadPOD<s_int_8>();
}

u_int_16    GpByteReader::GpByteReader::UInt16 (void)
{
    return ReadPOD<u_int_16>();
}

s_int_16    GpByteReader::GpByteReader::SInt16 (void)
{
    return ReadPOD<s_int_16>();
}

u_int_32    GpByteReader::GpByteReader::UInt32 (void)
{
    return ReadPOD<u_int_32>();
}

s_int_32    GpByteReader::GpByteReader::SInt32 (void)
{
    return ReadPOD<s_int_32>();
}

u_int_64    GpByteReader::GpByteReader::UInt64 (void)
{
    return ReadPOD<u_int_64>();
}

s_int_64    GpByteReader::GpByteReader::SInt64 (void)
{
    return ReadPOD<s_int_64>();
}

u_int_64    GpByteReader::CompactUInt64 (void)
{
    u_int_64 val = 0;

    for (ssize_t id = (sizeof(u_int_64) + 1); id >= 0; --id)
    {
        const u_int_8 byte = UInt8();
        val = val << 7;
        val |= byte & 0b01111111;

        if (!(byte & u_int_8(0b10000000)))
        {
            break;
        }
    }

    return val;
}

s_int_64    GpByteReader::CompactSInt64 (void)
{
    u_int_64 value = CompactUInt64();

    if (value & u_int_64(1))//Negative
    {
        if (value != 1)
        {
            return -s_int_64(value >> 1);
        } else
        {
            return std::numeric_limits<s_int_64>::min();
        }
    } else
    {
        return s_int_64(value >> 1);
    }
}

GpSpanPtrByteR  GpByteReader::BytesWithLen (void)
{
    //Length
    const size_t size = NumOps::SConvert<size_t>(CompactUInt64());

    //Data
    return Bytes(size);
}

}//GPlatform
