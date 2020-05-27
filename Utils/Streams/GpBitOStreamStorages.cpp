#include "GpBitOStreamStorages.hpp"

namespace GPlatform {

void	GpBitOStreamFixedSize::_AllocateNext (const size_bit_t aSize)
{
	if (iLeftToWrite < aSize)
	{
		THROW_GPE("Out of range"_sv);
	}
}

/*void	GpBitOStreamByteArray::_AllocateNext (const size_bit_t aSize)
{
	if (iLeftToWrite < aSize)
	{
		const count_t delta		= aSize - iLeftFreeBytes;
		const count_t newSize	= count_t::SMake(iOut.size()) + delta;

		iOut.resize(newSize.Value());
		iLeftFreeBytes	+= delta;
		iData			= iOut.data() + (count_t::SMake(iOut.size()) - iLeftFreeBytes).Value();
	}
}*/

}//GPlatform
