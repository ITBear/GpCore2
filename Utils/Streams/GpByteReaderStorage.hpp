#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpByteReaderStorage
{
	CLASS_REMOVE_CTRS(GpByteReaderStorage);

public:
	inline					GpByteReaderStorage		(const std::byte*	aData,
													 const size_byte_t	aSize) noexcept;
	inline					GpByteReaderStorage		(std::string_view	aData) noexcept;
	inline					GpByteReaderStorage		(const GpBytesArray& aData) noexcept;
							~GpByteReaderStorage	(void) noexcept {}

	const std::byte*		Data					(void) const noexcept {return iData;}
	size_byte_t				Size					(void) const noexcept {return iSize;}
	size_byte_t				Left					(void) const noexcept {return iLeft;}
	void					SetLeftSub				(const size_byte_t aValue) {iLeft -= aValue;}

private:
	const std::byte*		iData = nullptr;
	const size_byte_t		iSize;
	size_byte_t				iLeft;
};

GpByteReaderStorage::GpByteReaderStorage (const std::byte*	aData,
										  const size_byte_t	aSize) noexcept:
iData(aData),
iSize(aSize),
iLeft(iSize)
{
}

GpByteReaderStorage::GpByteReaderStorage (std::string_view aData) noexcept:
GpByteReaderStorage(reinterpret_cast<const std::byte*>(aData.data()),
					size_byte_t::SMake(aData.size()))
{
}

GpByteReaderStorage::GpByteReaderStorage (const GpBytesArray& aData) noexcept:
GpByteReaderStorage(aData.data(),
					size_byte_t::SMake(aData.size()))
{
}

}//GPlatform
