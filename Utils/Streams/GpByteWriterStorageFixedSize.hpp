#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorageFixedSize final: public GpByteWriterStorage
{
	CLASS_REMOVE_CTRS(GpByteWriterStorageFixedSize);

public:
	inline					GpByteWriterStorageFixedSize	(std::byte*			aData,
															 const size_byte_t	aSize) noexcept;
	virtual					~GpByteWriterStorageFixedSize	(void) noexcept override final;

	virtual void			AllocateNext					(const size_byte_t aSize) override final;
};

GpByteWriterStorageFixedSize::GpByteWriterStorageFixedSize (std::byte*			aData,
															const size_byte_t	aSize) noexcept:
GpByteWriterStorage(aData, aSize)
{
}

}//GPlatform
