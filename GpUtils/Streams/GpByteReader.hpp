#pragma once

#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>

namespace GPlatform {

class GP_UTILS_API GpByteReader
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteReader)

public:
    inline                  GpByteReader            (GpByteReaderStorage& aStorage) noexcept;
                            ~GpByteReader           (void) noexcept = default;

    inline size_t           SizeLeft                (void) const noexcept;
    inline size_t           TotalRead               (void) const noexcept;
    inline GpSpanByteR      OffsetAdd               (size_t aOffset);// Same as Bytes(aOffset)
    inline GpSpanByteR      StoragePtr              (void) noexcept;

    inline u_int_8          UI8                     (void);
    inline s_int_8          SI8                     (void);
    inline u_int_16         UI16                    (void);
    inline s_int_16         SI16                    (void);
    inline u_int_32         UI32                    (void);
    inline s_int_32         SI32                    (void);
    inline u_int_64         UI64                    (void);
    inline s_int_64         SI64                    (void);
    u_int_64                CompactUI64             (void);
    s_int_64                CompactSI64             (void);
    GpSpanByteR             BytesWithLen            (void);
    inline GpSpanByteR      Bytes                   (size_t aSize);
    GpSpanByteR             TextLine                (void);
    std::string_view        NullTerminatedString    (void);

private:
    template<typename T>
    T                       ReadPOD         (void);

private:
    GpByteReaderStorage&    iStorage;
};

GpByteReader::GpByteReader (GpByteReaderStorage& aStorage) noexcept:
iStorage{aStorage}
{
}

size_t  GpByteReader::SizeLeft (void) const noexcept
{
    return iStorage.SizeLeft();
}

size_t  GpByteReader::TotalRead (void) const noexcept
{
    return iStorage.TotalRead();
}

GpSpanByteR GpByteReader::OffsetAdd (const size_t aOffset)
{
    return Bytes(aOffset);
}

GpSpanByteR GpByteReader::StoragePtr (void) noexcept
{
    return iStorage.StoragePtr();
}

u_int_8 GpByteReader::GpByteReader::UI8 (void)
{
    return ReadPOD<u_int_8>();
}

s_int_8 GpByteReader::GpByteReader::SI8 (void)
{
    return ReadPOD<s_int_8>();
}

u_int_16    GpByteReader::GpByteReader::UI16 (void)
{
    return ReadPOD<u_int_16>();
}

s_int_16    GpByteReader::GpByteReader::SI16 (void)
{
    return ReadPOD<s_int_16>();
}

u_int_32    GpByteReader::GpByteReader::UI32 (void)
{
    return ReadPOD<u_int_32>();
}

s_int_32    GpByteReader::GpByteReader::SI32 (void)
{
    return ReadPOD<s_int_32>();
}

u_int_64    GpByteReader::GpByteReader::UI64 (void)
{
    return ReadPOD<u_int_64>();
}

s_int_64    GpByteReader::GpByteReader::SI64 (void)
{
    return ReadPOD<s_int_64>();
}

GpSpanByteR GpByteReader::Bytes (const size_t aSize)
{
    return iStorage.Read(aSize);
}

template<typename T>
T   GpByteReader::ReadPOD (void)
{
    GpSpanByteR data = Bytes(sizeof(T));

    T val;
    std::memcpy(&val, data.Ptr(), sizeof(T));
    val = BitOps::H2N(val);

    return val;
}

}// namespace GPlatform
