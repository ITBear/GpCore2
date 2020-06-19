#include "GpBitWriterStorageFixedSize.hpp"

namespace GPlatform {

GpBitWriterStorageFixedSize::~GpBitWriterStorageFixedSize (void) noexcept
{
}

void	GpBitWriterStorageFixedSize::AllocateNext (const size_bit_t aSize)
{
    if (Left() < aSize)
    {
        THROW_GPE("Out of range"_sv);
    }
}

}//namespace GPlatform
