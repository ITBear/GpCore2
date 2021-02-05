#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_BASE64)

#include "../../Types/GpTypes.hpp"
#include "../Streams/GpStreams.hpp"

namespace GPlatform {

class GPCORE_API GpBase64
{
    CLASS_REMOVE_CTRS(GpBase64)

public:
    static size_byte_t      SEncode             (GpRawPtrByteR  aSrcData,
                                                 GpByteWriter&  aWriterBase64Str,
                                                 const count_t  aSingleLineMaxLength);
    static std::string      SEncode             (GpRawPtrByteR  aData,
                                                 const count_t  aSingleLineMaxLength);

    static size_byte_t      SDecode             (GpRawPtrCharR  aBase64Str,
                                                 GpByteWriter&  aWriterData);
    static std::string      SDecode             (GpRawPtrByteR  aBase64Str);

private:
    static size_byte_t      SCalcEncodedSize    (GpRawPtrByteR  aSrcData,
                                                 const count_t  aSingleLineMaxLength);
    static count_t          SCalcBlocksCount    (GpRawPtrByteR  aSrcData);

    static u_int_8          SAlphabetToByte     (const u_int_8  aValue,
                                                 size_t&        aBlockPartsRead,
                                                 bool&          aIsEnd);

    static const GpArray<std::byte, 64> sAlphabet;
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
