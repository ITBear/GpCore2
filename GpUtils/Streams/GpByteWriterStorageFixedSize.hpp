#pragma once

#include <GpCore2/GpUtils/Streams/GpByteWriterStorage.hpp>

namespace GPlatform {

class GP_UTILS_API GpByteWriterStorageFixedSize final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorageFixedSize)

public:
    inline          GpByteWriterStorageFixedSize    (GpSpanByteRW aDataOut) noexcept;
    virtual         ~GpByteWriterStorageFixedSize   (void) noexcept override final = default;

protected:
    virtual void    AllocateAdd                     (size_t         aSizeToAdd,
                                                     GpSpanByteRW&  aStoragePtr) override final;
    virtual void    _OnEnd                          (void) override final;
};

GpByteWriterStorageFixedSize::GpByteWriterStorageFixedSize (GpSpanByteRW aDataOut) noexcept:
GpByteWriterStorage{aDataOut}
{
}

}// namespace GPlatform
