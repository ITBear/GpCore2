#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_BASE64)

#include "../../Types/GpTypes.hpp"
#include "../Streams/GpStreams.hpp"

namespace GPlatform {

class GPCORE_API GpBase64
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase64)

public:
    static void             SEncode             (GpRawPtrByteR  aData,
                                                 GpByteWriter&  aWriterBase64Str,
                                                 const count_t  aSingleLineMaxLength);
    static std::string      SEncodeToStr        (GpRawPtrByteR  aData,
                                                 const count_t  aSingleLineMaxLength);
    static GpBytesArray     SEncodeToByteArray  (GpRawPtrByteR  aData,
                                                 const count_t  aSingleLineMaxLength);

    static void             SDecode             (GpRawPtrCharR  aBase64Str,
                                                 GpByteWriter&  aWriterData);
    static std::string      SDecodeToStr        (GpRawPtrCharR  aBase64Str);
    static GpBytesArray     SDecodeToByteArray  (GpRawPtrCharR  aBase64Str);

private:
    static size_byte_t      SEncodedSize        (GpRawPtrByteR  aData,
                                                 const count_t  aSingleLineMaxLength);
    static size_byte_t      SDecodedSize        (GpRawPtrCharR  aBase64Str);
    static count_t          SEncodedBlocksCount (GpRawPtrByteR  aData);

    static std::byte        SAlphabetToByte     (const std::byte aValue);

    static const GpArray<std::byte, 64> sAlphabet;
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
