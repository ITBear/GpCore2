#include "GpBitWriterStorage.hpp"

namespace GPlatform {

GpBitWriterStorage::~GpBitWriterStorage	(void) noexcept
{
}

void	GpBitWriterStorage::AllocateNext (const size_bit_t aSize)
{
    if (iLeft < aSize)
    {
        THROW_GPE("Out of range"_sv);
    }
}

}//GPlatform
