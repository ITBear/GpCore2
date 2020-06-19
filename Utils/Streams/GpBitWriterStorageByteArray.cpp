#include "GpBitWriterStorageByteArray.hpp"

namespace GPlatform {

GpBitWriterStorageByteArray::~GpBitWriterStorageByteArray (void) noexcept
{
}

void	GpBitWriterStorageByteArray::AllocateNext (const size_bit_t aSize)
{
	const size_bit_t left = Left();

	if (left >= aSize)
	{
		return;
	}

	size_bit_t	delta		= aSize - left;
	size_bit_t	newSize		= size_byte_t::SMake(iOut.size()) + delta;
	size_bit_t	m			= newSize % 8_bit;

	if (m > 0_bit)
	{
		delta	+= m;
		newSize	+= m;
	}

	const size_byte_t allocSize = newSize;

	iOut.resize(allocSize.ValueAs<size_t>());

	SetSize(newSize);
	SetLeft(left + delta);
	SetData(iOut.data() + (size_byte_t::SMake(iOut.size()) - Left()).ValueAs<size_t>());
}

}//namespace GPlatform
