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

GpRawPtrByteR   GpByteReader::BytesWithLen (void)
{
    //Length
    const size_byte_t size = size_byte_t::SMake(CompactSInt32());

    //Data
    return Bytes(size);
}

s_int_32    GpByteReader::CompactSInt32 (void)
{
    s_int_32 val = 0;

    for (ssize_t id = 3; id >= 0; --id)
    {
        const u_int_8 byte = UInt8();
        val = val << 7;
        val |= byte & 0b01111111;

        if (byte & 0b1000000)
        {
            break;
        }
    }

    return val;
}

}//GPlatform
