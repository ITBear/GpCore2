#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorageByteArray final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS(GpByteWriterStorageByteArray);

public:
    inline					GpByteWriterStorageByteArray	(GpBytesArray& aOut) noexcept;
    virtual					~GpByteWriterStorageByteArray	(void) noexcept override final;

    virtual void			AllocateNext					(const size_byte_t aSize) override final;

private:
    GpBytesArray&			iOut;
};

GpByteWriterStorageByteArray::GpByteWriterStorageByteArray (GpBytesArray& aOut) noexcept:
GpByteWriterStorage(aOut.data(), size_byte_t::SMake(aOut.size())),
iOut(aOut)
{
}

}//GPlatform
