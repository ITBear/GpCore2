#include "GpByteOStreamStorages.hpp"

namespace GPlatform {

void	GpByteOStreamFixedSize::_AllocateNext (const count_t aSize)
{
	if (iLeftFreeBytes < aSize)
	{
		THROW_GPE("Out of range"_sv);
	}
}

void	GpByteOStreamByteArray::_AllocateNext (const count_t aSize)
{
	if (iLeftFreeBytes < aSize)
	{
		const count_t delta		= aSize - iLeftFreeBytes;
		const count_t newSize	= count_t::SMake(iOut.size()) + delta;

		iOut.resize(newSize.Value());
		iLeftFreeBytes	+= delta;
		iData			= iOut.data() + (count_t::SMake(iOut.size()) - iLeftFreeBytes).Value();
	}
}

}//GPlatform
