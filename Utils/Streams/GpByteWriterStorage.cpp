#include "GpByteWriterStorage.hpp"

namespace GPlatform {

GpByteWriterStorage::~GpByteWriterStorage (void) noexcept
{
}

void	GpByteWriterStorage::AllocateNext (const size_byte_t aSize)
{
	if (iLeft < aSize)
	{
		THROW_GPE("Out of range"_sv);
	}
}

}//GPlatform
