#pragma once

#include "GpByteReaderStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteReader
{
    CLASS_REMOVE_CTRS(GpByteReader);

public:
                            GpByteReader	(GpByteReaderStorage& aStorage) noexcept: iStorage(aStorage){}
                            ~GpByteReader	(void) noexcept = default;

    u_int_8					UInt8			(void);
    s_int_8					SInt8			(void);
    u_int_16				UInt16			(void);
    s_int_16				SInt16			(void);
    u_int_32				UInt32			(void);
    s_int_32				SInt32			(void);
    u_int_64				UInt64			(void);
    s_int_64				SInt64			(void);
    std::string_view		BytesWithLen	(void);
    std::string_view		Bytes			(const size_byte_t aSize);

    s_int_32				CompactSInt32	(void);

private:
    template<typename T>
    T						ReadPOD			(void)
    {
        std::string_view data = Bytes(size_byte_t::SMake(sizeof(T)));

        T val;
        std::memcpy(&val, data.data(), data.size());
        val = BitOps::H2N(val);
        return val;
    }

private:
    GpByteReaderStorage&	iStorage;
};

}//GPlatform
