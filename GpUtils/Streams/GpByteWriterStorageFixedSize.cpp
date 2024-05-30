#include "GpByteWriterStorageFixedSize.hpp"

namespace GPlatform {

void    GpByteWriterStorageFixedSize::AllocateAdd
(
    const size_t    /*aSizeToAdd*/,
    GpSpanByteRW&   /*aStoragePtr*/
)
{
    THROW_GP("Out of range"_sv);
}

void    GpByteWriterStorageFixedSize::_OnEnd (void)
{
    //NOP
}

}// namespace GPlatform
