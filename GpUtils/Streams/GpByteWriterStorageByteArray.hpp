#pragma once

#include <GpCore2/GpUtils/Streams/GpByteWriterStorage.hpp>

namespace GPlatform {

class GP_UTILS_API GpByteWriterStorageByteArray final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorageByteArray)

public:
    inline          GpByteWriterStorageByteArray    (GpBytesArray& aStorage) noexcept;
    virtual         ~GpByteWriterStorageByteArray   (void) noexcept override final = default;

protected:
    virtual void    AllocateAdd                     (size_t         aSizeToAdd,
                                                     GpSpanByteRW&  aStoragePtr) override final;
    virtual void    _OnEnd                          (void) override final;

private:
    GpBytesArray&   iStorage;
};

GpByteWriterStorageByteArray::GpByteWriterStorageByteArray (GpBytesArray& aStorage) noexcept:
GpByteWriterStorage
{
    GpSpanByteRW
    {
        std::data(aStorage),
        std::size(aStorage)
    }
},
iStorage{aStorage}
{
}

}// namespace GPlatform
