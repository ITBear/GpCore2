#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpByteReaderStorage
{
public:
    inline                      GpByteReaderStorage     (GpRawPtrByteR aData) noexcept;
    inline                      GpByteReaderStorage     (const GpByteReaderStorage& aStorage) noexcept;
    inline                      GpByteReaderStorage     (GpByteReaderStorage&& aStorage) noexcept;
                                ~GpByteReaderStorage    (void) noexcept {}

    inline GpByteReaderStorage& operator=               (const GpByteReaderStorage& aStorage) noexcept;
    inline GpByteReaderStorage& operator=               (GpByteReaderStorage&& aStorage) noexcept;

    GpRawPtrByteR               ReadAndShift            (const size_byte_t aSize);
    GpRawPtrByteR               TryReadAndShift         (const size_byte_t aSize);
    size_byte_t                 SizeLeft                (void) const noexcept {return iData.SizeLeft();}
    void                        Rollback                (const size_byte_t aSize) {iData.OffsetSub(aSize.As<count_t>());}

protected:
    GpRawPtrByteR               iData;
};

GpByteReaderStorage::GpByteReaderStorage (GpRawPtrByteR aData) noexcept:
iData(aData)
{
}

GpByteReaderStorage::GpByteReaderStorage (const GpByteReaderStorage& aStorage) noexcept:
iData(aStorage.iData)
{
}

GpByteReaderStorage::GpByteReaderStorage (GpByteReaderStorage&& aStorage) noexcept:
iData(std::move(aStorage.iData))
{
}

GpByteReaderStorage&    GpByteReaderStorage::operator= (const GpByteReaderStorage& aStorage) noexcept
{
    iData = aStorage.iData;
    return *this;
}

GpByteReaderStorage&    GpByteReaderStorage::operator= (GpByteReaderStorage&& aStorage) noexcept
{
    iData = std::move(aStorage.iData);
    return *this;
}

}//GPlatform
