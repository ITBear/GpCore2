#include "GpByteWriterStorageFixedSize.hpp"

namespace GPlatform {

void    GpByteWriterStorageFixedSize::AllocateAdd
(
    const size_t        /*aSizeToAdd*/,
    GpSpanPtrByteRW&    /*aStoragePtr*/
)
{
    THROW_GP(u8"Out of range"_sv);
}

void    GpByteWriterStorageFixedSize::_OnEnd (void)
{
    //NOP
}

}// namespace GPlatform
