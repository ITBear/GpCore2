#include "GpTypeMapper.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../../Utils/Streams/GpByteWriter.hpp"
#include "../../Utils/Streams/GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

GpTypeMapper::GpTypeMapper (void) noexcept
{
}

GpTypeMapper::~GpTypeMapper (void) noexcept
{
}

GpBytesArray    GpTypeMapper::FromStruct (const GpTypeStructBase& aStruct) const
{
    GpBytesArray data;
    data.reserve(512);

    GpByteWriterStorageByteArray    dataStorage(data);
    GpByteWriter                    dataWriter(dataStorage);

    FromStruct(aStruct, dataWriter);

    return data;
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
