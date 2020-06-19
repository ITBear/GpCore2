#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorage
{
    CLASS_REMOVE_CTRS(GpByteWriterStorage);

protected:
    inline					GpByteWriterStorage	(std::byte*			aData,
                                                 const size_byte_t	aSize) noexcept;

public:
    virtual					~GpByteWriterStorage(void) noexcept;

    const std::byte*		Data				(void) const noexcept {return iData;}
    std::byte*				Data				(void) noexcept {return iData;}
    size_byte_t				Size				(void) const noexcept {return iSize;}
    size_byte_t				Left				(void) const noexcept {return iLeft;}
    void					SetLeftSub			(const size_byte_t aValue) {iLeft -= aValue;}

    virtual void			AllocateNext		(const size_byte_t aSize);

protected:
    void					SetData				(std::byte*			aData) noexcept {iData = aData;}
    void					SetSize				(const size_byte_t	aValue)noexcept {iSize = aValue;}
    void					SetLeft				(const size_byte_t	aValue)noexcept {iLeft = aValue;}

private:
    std::byte*				iData = nullptr;
    size_byte_t				iSize;
    size_byte_t				iLeft;
};

GpByteWriterStorage::GpByteWriterStorage (std::byte*		aData,
                                          const size_byte_t	aSize) noexcept:
iData(aData),
iSize(aSize),
iLeft(iSize)
{
}

}//GPlatform
