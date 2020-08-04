#include "GpBitWriterStorage.hpp"

namespace GPlatform {

GpBitWriterStorage::~GpBitWriterStorage (void) noexcept
{
}

void    GpBitWriterStorage::AllocateNext (const size_bit_t aSize)
{
    THROW_GPE_COND_CHECK_M(iLeft >= aSize, "Out of range"_sv);
}

}//GPlatform
