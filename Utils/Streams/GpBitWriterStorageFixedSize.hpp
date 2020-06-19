#pragma once

#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitWriterStorageFixedSize final: public GpBitWriterStorage
{
	CLASS_REMOVE_CTRS(GpBitWriterStorageFixedSize);

public:
	inline					GpBitWriterStorageFixedSize		(std::byte*			aData,
															 const size_bit_t	aSize) noexcept;
	inline					GpBitWriterStorageFixedSize		(std::byte*			aData,
															 const size_bit_t	aSize,
															 const size_bit_t	aOffset) noexcept;
	virtual					~GpBitWriterStorageFixedSize	(void) noexcept override final;

	virtual void			AllocateNext					(const size_bit_t aSize) override final;
};

GpBitWriterStorageFixedSize::GpBitWriterStorageFixedSize (std::byte*		aData,
														  const size_bit_t	aSize) noexcept:
GpBitWriterStorage(aData, aSize)
{
}

GpBitWriterStorageFixedSize::GpBitWriterStorageFixedSize (std::byte*		aData,
														  const size_bit_t	aSize,
														  const size_bit_t	aOffset) noexcept:
GpBitWriterStorage(aData, aSize, aOffset)
{
}

}//GPlatform
