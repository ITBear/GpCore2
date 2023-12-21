#pragma once

#include "GpByteReaderStorage.hpp"
#include "../Types/Bits/GpBitOps.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteReader
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteReader)

public:
    enum class Mode
    {
        N2H,    // Network to host conversion
        NO_N2H  // NO network to host conversion
    };

public:
    inline                  GpByteReader    (GpByteReaderStorage& aStorage) noexcept;
                            ~GpByteReader   (void) noexcept = default;

    inline size_t           SizeLeft        (void) const noexcept;
    inline GpSpanPtrByteR   Offset          (const size_t aOffset);//Same as Bytes(aOffset)
    inline GpSpanPtrByteR   StoragePtr      (void) noexcept;

    inline u_int_8          UInt8           (const Mode aMode = Mode::N2H);
    inline s_int_8          SInt8           (const Mode aMode = Mode::N2H);
    inline u_int_16         UInt16          (const Mode aMode = Mode::N2H);
    inline s_int_16         SInt16          (const Mode aMode = Mode::N2H);
    inline u_int_32         UInt32          (const Mode aMode = Mode::N2H);
    inline s_int_32         SInt32          (const Mode aMode = Mode::N2H);
    inline u_int_64         UInt64          (const Mode aMode = Mode::N2H);
    inline s_int_64         SInt64          (const Mode aMode = Mode::N2H);
    u_int_64                CompactUInt64   (void);
    s_int_64                CompactSInt64   (void);
    GpSpanPtrByteR          BytesWithLen    (void);
    inline GpSpanPtrByteR   Bytes           (const size_t aSize);
    GpSpanPtrByteR          NextTextLine    (void);

private:
    template<typename T>
    T                       ReadPOD         (const Mode aMode)
    {
        GpSpanPtrByteR data = Bytes(sizeof(T));

        T val;
        std::memcpy(&val, data.Ptr(), sizeof(T));

        if (aMode == Mode::N2H)
        {
            val = BitOps::H2N(val);
        }

        return val;
    }

private:
    GpByteReaderStorage&    iStorage;
};

GpByteReader::GpByteReader (GpByteReaderStorage& aStorage) noexcept:
iStorage(aStorage)
{
}

size_t  GpByteReader::SizeLeft (void) const noexcept
{
    return iStorage.SizeLeft();
}

GpSpanPtrByteR  GpByteReader::Offset (const size_t aOffset)
{
    return Bytes(aOffset);
}

GpSpanPtrByteR  GpByteReader::StoragePtr (void) noexcept
{
    return iStorage.StoragePtr();
}

u_int_8 GpByteReader::GpByteReader::UInt8 (const Mode aMode)
{
    return ReadPOD<u_int_8>(aMode);
}

s_int_8 GpByteReader::GpByteReader::SInt8 (const Mode aMode)
{
    return ReadPOD<s_int_8>(aMode);
}

u_int_16    GpByteReader::GpByteReader::UInt16 (const Mode aMode)
{
    return ReadPOD<u_int_16>(aMode);
}

s_int_16    GpByteReader::GpByteReader::SInt16 (const Mode aMode)
{
    return ReadPOD<s_int_16>(aMode);
}

u_int_32    GpByteReader::GpByteReader::UInt32 (const Mode aMode)
{
    return ReadPOD<u_int_32>(aMode);
}

s_int_32    GpByteReader::GpByteReader::SInt32 (const Mode aMode)
{
    return ReadPOD<s_int_32>(aMode);
}

u_int_64    GpByteReader::GpByteReader::UInt64 (const Mode aMode)
{
    return ReadPOD<u_int_64>(aMode);
}

s_int_64    GpByteReader::GpByteReader::SInt64 (const Mode aMode)
{
    return ReadPOD<s_int_64>(aMode);
}

GpSpanPtrByteR  GpByteReader::Bytes (const size_t aSize)
{
    return iStorage.Read(aSize);
}

}//GPlatform
