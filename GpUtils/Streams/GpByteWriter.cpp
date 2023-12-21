#include "GpByteWriter.hpp"

namespace GPlatform {

GpByteWriter&   GpByteWriter::BytesWithLen (GpSpanPtrByteR aData)
{
    const u_int_64 size = NumOps::SConvert<u_int_64>(aData.Count());
    CompactUInt64(size);

    if (size > 0)
    {
        Bytes(aData);
    }

    return *this;
}

GpByteWriter&   GpByteWriter::CompactUInt64 (const u_int_64 aValue)
{
    std::array<u_int_8, sizeof(u_int_64) + 2> buf;

    size_t      i       = buf.size();
    u_int_64    value   = aValue;

    do
    {
        buf.data()[--i] = u_int_8(value & u_int_64(0b01111111));
        value = value >> 7;
    } while (value > 0);

    while (i < buf.size())
    {
        if (i != (buf.size() - 1))
        {
            UInt8(u_int_8(buf[i++]) | u_int_8(0b10000000));
        } else
        {
            UInt8(u_int_8(buf[i++]));
        }
    }

    return *this;
}

GpByteWriter&   GpByteWriter::CompactSInt64 (const s_int_64 aValue)
{
    u_int_64 value = std::bit_cast<u_int_64>(aValue);

    if (aValue >= 0)
    {
        value = (value << 1);
    } else
    {
        value = ((u_int_64(-(aValue+1)) + 1) << 1) | 1;
    }

    return CompactUInt64(value);
}

}//GPlatform
