#include <GpCore2/GpReflection/Serializers/GpReflectSerializer.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>

namespace GPlatform {

GpByteArray GpReflectSerializer::FromObject (const GpReflectObject& aObject) const
{
    GpByteArray data;
    data.resize(512);

    {
        GpByteWriterStorageByteArray    dataStorage(data);
        GpByteWriter                    dataWriter(dataStorage);

        FromObject(aObject, dataWriter);

        dataWriter.OnEnd();
    }

    return data;
}

}// namespace GPlatform
