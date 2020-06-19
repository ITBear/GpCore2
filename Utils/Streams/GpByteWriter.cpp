#include "GpByteWriter.hpp"

namespace GPlatform {

void	GpByteWriter::UInt8 (const u_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::SInt8 (const s_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::UInt16 (const u_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::SInt16 (const s_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::UInt32 (const u_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::SInt32 (const s_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::UInt64 (const u_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::SInt64 (const s_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
}

void	GpByteWriter::BytesWithLen (std::string_view aValue)
{
    BytesWithLen(reinterpret_cast<const std::byte*>(aValue.data()),
                  size_byte_t::SMake(aValue.size()));
}

void	GpByteWriter::Bytes (std::string_view aValue)
{
    Bytes(reinterpret_cast<const std::byte*>(aValue.data()),
          size_byte_t::SMake(aValue.size()));
}

void	GpByteWriter::BytesWithLen (const GpBytesArray& aValue)
{
    BytesWithLen(aValue.data(),
                 size_byte_t::SMake(aValue.size()));
}

void	GpByteWriter::Bytes (const GpBytesArray& aValue)
{
    Bytes(aValue.data(),
          size_byte_t::SMake(aValue.size()));
}

void	GpByteWriter::BytesWithLen (const std::byte* aData, const size_byte_t aSize)
{
    CompactSInt32(aSize.ValueAs<s_int_32>());
    Bytes(aData, aSize);
}

void	GpByteWriter::Bytes (const std::byte* aData,  const size_byte_t aSize)
{
    if ((aData == nullptr) || (aSize == 0_byte))
    {
        return;
    }

    //Data
    size_bit_t left = iStorage.Left();

    if (left < aSize)
    {
        iStorage.AllocateNext(aSize);
        left = iStorage.Left();
    }

    std::memcpy(iStorage.Data() + (iStorage.Size() - left).ValueAs<size_t>(),
                aData,
                aSize.ValueAs<size_t>());

    iStorage.SetLeftSub(aSize);
}

void	GpByteWriter::CompactSInt32 (const s_int_32 aValue)
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
