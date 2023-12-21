#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteWriterStorageFixedSize final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorageFixedSize)

public:
    inline                  GpByteWriterStorageFixedSize    (GpSpanPtrByteRW aDataOut) noexcept;
    virtual                 ~GpByteWriterStorageFixedSize   (void) noexcept override final = default;

protected:
    virtual void            AllocateAdd                     (const size_t       aSizeToAdd,
                                                             GpSpanPtrByteRW&   aStoragePtr) override final;
    virtual void            _OnEnd                          (void) override final;
};

GpByteWriterStorageFixedSize::GpByteWriterStorageFixedSize (GpSpanPtrByteRW aDataOut) noexcept:
GpByteWriterStorage(aDataOut)
{
}

}// namespace GPlatform
