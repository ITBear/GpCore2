#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_BASE58)

#include "../../Types/GpTypes.hpp"
#include "../Streams/GpStreams.hpp"
#include <any>

namespace GPlatform {

GP_ENUM(GPCORE_API, GpBase58Alphabet,
    NAKAMOTO,
    SHORT_URL
);

class GPCORE_API GpBase58
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase58)

public:
    using AlphabetT     = GpBase58Alphabet;
    using AlphabetTE    = AlphabetT::EnumT;

public:
    static void             SEncode             (GpRawPtrByteR      aData,
                                                 GpByteWriter&      aWriterBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static std::string      SEncodeToStr        (GpRawPtrByteR      aData,
                                                 const AlphabetTE   aAlphabet);
    static GpBytesArray     SEncodeToByteArray  (GpRawPtrByteR      aData,
                                                 const AlphabetTE   aAlphabet);

    static void             SDecode             (GpRawPtrCharR      aBase58Str,
                                                 GpByteWriter&      aWriterData,
                                                 const AlphabetTE   aAlphabet);
    static std::string      SDecodeToStr        (GpRawPtrCharR      aBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static GpBytesArray     SDecodeToByteArray  (GpRawPtrCharR      aBase58Str,
                                                 const AlphabetTE   aAlphabet);

private:
    static const auto&      Alphabet            (GpBase58Alphabet   aAlphabet) noexcept
    {
        return sAlphabets[aAlphabet.ID()];
    }

    static size_byte_t      SEncodedSize        (GpRawPtrByteR aData);
    static std::any         SDecodePrecalc      (GpRawPtrCharR      aBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static void             SDecode             (const std::any&    aDecodePrecalc,
                                                 GpByteWriter&      aWriterData);
    static size_t           SFindChId           (const u_int_8                  aCh,
                                                 const GpArray<u_int_8, 58>&    aAlphabet);

private:
    static GpArray<u_int_8, 58> sAlphabets[GpBase58Alphabet::SCount().As<size_t>()];
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
