#include "GpByteWriterStorageFixedSize.hpp"

namespace GPlatform {

void    GpByteWriterStorageFixedSize::AllocateAdd
(
    const size_t        /*aSizeToAdd*/,
    GpSpanPtrByteRW&    /*aStoragePtr*/
)
{
    THROW_GP("Out of range"_sv);
}

void    GpByteWriterStorageFixedSize::OnShrinkToFit (void)
{
    //NOP
}

}//namespace GPlatform
