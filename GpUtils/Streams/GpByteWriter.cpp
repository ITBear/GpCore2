#include "GpByteWriter.hpp"

namespace GPlatform {

GpByteWriter&   GpByteWriter::BytesWithLen (GpSpanByteR aData)
{
    const u_int_64 size = NumOps::SConvert<u_int_64>(aData.Count());
    CompactUI64(size);

    if (size > 0)
    {
        Bytes(aData);
    }

    return *this;
}

GpByteWriter&   GpByteWriter::CompactUI64 (const u_int_64 aValue)
{
    std::array<u_int_8, sizeof(u_int_64) + 2> buf;

    size_t      i       = std::size(buf);
    u_int_64    value   = aValue;

    do
    {
        std::data(buf)[--i] = u_int_8(value & u_int_64(0b01111111));
        value = value >> 7;
    } while (value > 0);

    while (i < std::size(buf))
    {
        if (i != (std::size(buf) - 1))
        {
            UI8(u_int_8(buf[i++]) | u_int_8(0b10000000));
        } else
        {
            UI8(u_int_8(buf[i++]));
        }
    }

    return *this;
}

GpByteWriter&   GpByteWriter::CompactSI64 (const s_int_64 aValue)
{
    u_int_64 value = std::bit_cast<u_int_64>(aValue);

    if (aValue >= 0)
    {
        value = (value << 1);
    } else
    {
        value = ((u_int_64(-(aValue+1)) + 1) << 1) | 1;
    }

    return CompactUI64(value);
}

}// namespace GPlatform
