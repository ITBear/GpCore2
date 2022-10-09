#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_BASE64)

#include "../Streams/GpStreams.hpp"

namespace GPlatform {

class GP_UTILS_API GpBase64
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase64)

public:
    static void             SEncode             (GpSpanPtrByteR aData,
                                                 GpByteWriter&  aWriterBase64Str,
                                                 const size_t   aSingleLineMaxLength);
    static std::string      SEncodeToStr        (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);
    static GpBytesArray     SEncodeToByteArray  (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);

    static void             SDecode             (std::string_view   aBase64Str,
                                                 GpByteWriter&      aWriterData);
    static std::string      SDecodeToStr        (std::string_view   aBase64Str);
    static GpBytesArray     SDecodeToByteArray  (std::string_view   aBase64Str);

private:
    static size_t           SEncodedSize        (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);
    static size_t           SDecodedSize        (std::string_view   aBase64Str);
    static size_t           SEncodedBlocksCount (GpSpanPtrByteR     aData);

    static std::byte        SAlphabetToByte     (const std::byte aValue);

    static const std::array<std::byte, 64>  sAlphabet;
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
