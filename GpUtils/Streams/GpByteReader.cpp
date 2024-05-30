#include "GpByteReader.hpp"

namespace GPlatform {

u_int_64    GpByteReader::CompactUI64 (void)
{
    u_int_64 val = 0;

    for (ssize_t id = (sizeof(u_int_64) + 1); id >= 0; --id)
    {
        const u_int_8 byte = UI8();
        val = val << 7;
        val |= byte & 0b01111111;

        if (!(byte & u_int_8(0b10000000)))
        {
            break;
        }
    }

    return val;
}

s_int_64    GpByteReader::CompactSI64 (void)
{
    u_int_64 value = CompactUI64();

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

GpSpanByteR GpByteReader::BytesWithLen (void)
{
    //Length
    const size_t size = NumOps::SConvert<size_t>(CompactUI64());

    //Data
    return Bytes(size);
}

GpSpanByteR GpByteReader::NextTextLine (void)
{
    const char* _R_ dataPtr     = iStorage.StoragePtr().PtrAs<const char*>();
    const size_t    sizeLeft    = SizeLeft();

    for (size_t id = 0; id < sizeLeft; id++)
    {
        const char ch = dataPtr[id];

        if (ch == '\n') [[unlikely]]
        {
            return iStorage.Read(id+1).SubspanBegin(0, id);
        } else if (ch == '\r') [[unlikely]]
        {
            // Check next char
            if (   ((id+1) < sizeLeft)
                && (ch == '\n'))
            {
                return iStorage.Read(id+2).SubspanBegin(0, id);
            } else
            {
                return iStorage.Read(id+1).SubspanBegin(0, id);
            }
        }
    }

    // Read all data left
    return iStorage.Read(sizeLeft);
}

}// namespace GPlatform
