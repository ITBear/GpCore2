#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_BASE64)

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorage.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageFixedSize.hpp>

namespace GPlatform {

class GP_UTILS_API GpBase64
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase64)

public:
    static void             SEncode             (GpSpanByteR    aData,
                                                 GpByteWriter&  aWriterBase64Str,
                                                 size_t         aSingleLineMaxLength);
    template<typename T>
    static T                SEncode             (GpSpanByteR    aData,
                                                 size_t         aSingleLineMaxLength);

    static void             SDecode             (GpSpanByteR    aBase64Str,
                                                 GpByteWriter&  aWriterData);
    template<typename T>
    static T                SDecode             (GpSpanByteR    aBase64Str);

private:
    static size_t           SEncodedSize        (GpSpanByteR    aData,
                                                 size_t         aSingleLineMaxLength);
    static size_t           SDecodedSize        (GpSpanByteR    aBase64Str);
    static size_t           SEncodedBlocksCount (GpSpanByteR    aData);

    static u_int_8          SAlphabetToByte     (u_int_8 aValue);

    static const std::array<u_int_8, 64>    sAlphabet;
};

template<typename T>
T   GpBase64::SEncode
(
    GpSpanByteR     aData,
    const size_t    aSingleLineMaxLength
)
{
    const size_t encodedSize = SEncodedSize(aData, aSingleLineMaxLength);
    T containerOut;
    containerOut.resize(encodedSize);

    GpByteWriterStorageFixedSize    writerStorge{containerOut};
    GpByteWriter                    writer{writerStorge};

    SEncode(aData, writer, aSingleLineMaxLength);

    return containerOut;
}

template<typename T>
T   GpBase64::SDecode (GpSpanByteR aBase64Str)
{
    const size_t decodedSize = SDecodedSize(aBase64Str);
    T containerOut;
    containerOut.resize(decodedSize);

    GpByteWriterStorageFixedSize    writerStorge{containerOut};
    GpByteWriter                    writer{writerStorge};

    SDecode(aBase64Str, writer);

    return containerOut;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_BASE64)
