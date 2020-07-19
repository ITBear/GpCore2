#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriter
{
    CLASS_REMOVE_CTRS(GpByteWriter)

public:
                            GpByteWriter    (GpByteWriterStorage& aStorage) noexcept: iStorage(aStorage) {}
                            ~GpByteWriter   (void) noexcept = default;

    void                    UInt8           (const u_int_8 aValue);
    void                    SInt8           (const s_int_8 aValue);
    void                    UInt16          (const u_int_16 aValue);
    void                    SInt16          (const s_int_16 aValue);
    void                    UInt32          (const u_int_32 aValue);
    void                    SInt32          (const s_int_32 aValue);
    void                    UInt64          (const u_int_64 aValue);
    void                    SInt64          (const s_int_64 aValue);
    void                    BytesWithLen    (GpRawPtrByteR aData);
    void                    Bytes           (GpRawPtrByteR aData);
    void                    CompactSInt32   (const s_int_32 aValue);

private:
    template<typename T>
    void                    WritePOD        (const T aValue)
    {
        const count_t   size    = count_t::SMake(sizeof(T));
        const T         val     = BitOps::H2N(aValue);

        Bytes({reinterpret_cast<const std::byte*>(&val), size});
    }

private:
    GpByteWriterStorage&    iStorage;
};

}//GPlatform
