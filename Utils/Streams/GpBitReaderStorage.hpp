#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpBitReaderStorage
{
    CLASS_REMOVE_CTRS(GpBitReaderStorage);

public:
    inline					GpBitReaderStorage	(const std::byte*	aData,
                                                 const size_bit_t	aSize,
                                                 const size_bit_t	aOffset) noexcept;
    inline					GpBitReaderStorage	(std::string_view	aData) noexcept;
    inline					GpBitReaderStorage	(std::string_view	aData,
                                                 const size_bit_t	aOffset) noexcept;
    inline					GpBitReaderStorage	(const GpBytesArray&	aData) noexcept;
    inline					GpBitReaderStorage	(const GpBytesArray&	aData,
                                                 const size_bit_t		aOffset) noexcept;

                            ~GpBitReaderStorage	(void) noexcept {}

    const std::byte*		Data				(void) const noexcept {return iData;}
    size_bit_t				Size				(void) const noexcept {return iSize;}
    size_bit_t				Offset				(void) const noexcept {return iOffset;}
    size_bit_t				Left				(void) const noexcept {return iLeft;}
    void					SetLeftSub			(const size_bit_t aValue) {iLeft -= aValue;}

private:
    const std::byte*		iData = nullptr;
    const size_bit_t		iSize;
    const size_bit_t		iOffset;
    size_bit_t				iLeft;
};

GpBitReaderStorage::GpBitReaderStorage (const std::byte*	aData,
                                        const size_bit_t	aSize,
                                        const size_bit_t	aOffset) noexcept:
iData(aData),
iSize(aSize),
iOffset(aOffset),
iLeft(iSize)
{
}

GpBitReaderStorage::GpBitReaderStorage (std::string_view aData) noexcept:
GpBitReaderStorage(reinterpret_cast<const std::byte*>(aData.data()),
                   size_byte_t::SMake(aData.size()),
                   0_bit)
{
}

GpBitReaderStorage::GpBitReaderStorage (std::string_view	aData,
                                        const size_bit_t	aOffset) noexcept:
GpBitReaderStorage(reinterpret_cast<const std::byte*>(aData.data()),
                   size_byte_t::SMake(aData.size()),
                   aOffset)
{
}

GpBitReaderStorage::GpBitReaderStorage (const GpBytesArray&	aData) noexcept:
GpBitReaderStorage(aData.data(),
                   size_byte_t::SMake(aData.size()),
                   0_bit)
{
}

GpBitReaderStorage::GpBitReaderStorage (const GpBytesArray&	aData,
                                        const size_bit_t	aOffset) noexcept:
GpBitReaderStorage(aData.data(),
                   size_byte_t::SMake(aData.size()),
                   aOffset)
{
}

}//GPlatform
