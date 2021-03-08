#include "GpBitWriterStorage.hpp"

namespace GPlatform {

GpBitWriterStorage::~GpBitWriterStorage (void) noexcept
{
}

void    GpBitWriterStorage::AllocateNext (const size_bit_t aSize)
{
    THROW_GPE_COND
    (
        iLeft >= aSize,
        "Out of range"_sv
    );
}

}//GPlatform
