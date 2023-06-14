#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_BASE64)

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"
#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GpByteWriter;

class GP_UTILS_API GpBase64
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase64)

public:
    static void             SEncode             (GpSpanPtrByteR aData,
                                                 GpByteWriter&  aWriterBase64Str,
                                                 const size_t   aSingleLineMaxLength);
    static std::u8string    SEncodeToStr        (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);
    static GpBytesArray     SEncodeToByteArray  (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);

    static void             SDecode             (std::u8string_view aBase64Str,
                                                 GpByteWriter&      aWriterData);
    static std::u8string    SDecodeToStr        (std::u8string_view aBase64Str);
    static GpBytesArray     SDecodeToByteArray  (std::u8string_view aBase64Str);

private:
    static size_t           SEncodedSize        (GpSpanPtrByteR aData,
                                                 const size_t   aSingleLineMaxLength);
    static size_t           SDecodedSize        (std::u8string_view aBase64Str);
    static size_t           SEncodedBlocksCount (GpSpanPtrByteR     aData);

    static u_int_8          SAlphabetToByte     (const u_int_8 aValue);

    static const std::array<u_int_8, 64>    sAlphabet;
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
