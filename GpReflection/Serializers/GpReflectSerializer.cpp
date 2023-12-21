#include "GpReflectSerializer.hpp"

#if defined(GP_USE_REFLECTION)

#include "../../GpUtils/Streams/GpByteWriter.hpp"
#include "../../GpUtils/Streams/GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

GpBytesArray    GpReflectSerializer::FromObject (const GpReflectObject& aObject) const
{
    GpBytesArray data;
    data.resize(512);

    {
        GpByteWriterStorageByteArray    dataStorage(data);
        GpByteWriter                    dataWriter(dataStorage);

        FromObject(aObject, dataWriter);

        dataWriter.OnEnd();
    }

    return data;
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
