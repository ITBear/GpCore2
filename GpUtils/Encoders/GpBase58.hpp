#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_BASE58)

#include "../Streams/GpStreams.hpp"
#include "../Types/Enums/GpEnum.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/Strings/GpStringOps.hpp"

#include <any>

namespace GPlatform {

GP_ENUM(GP_UTILS_API, GpBase58Alphabet,
    NAKAMOTO,
    SHORT_URL
);

class GP_UTILS_API GpBase58
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBase58)

public:
    using AlphabetT     = GpBase58Alphabet;
    using AlphabetTE    = AlphabetT::EnumT;

public:
    static void             SEncode             (GpSpanPtrByteR     aData,
                                                 GpByteWriter&      aWriterBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static std::string      SEncodeToStr        (GpSpanPtrByteR     aData,
                                                 const AlphabetTE   aAlphabet);
    static GpBytesArray     SEncodeToByteArray  (GpSpanPtrByteR     aData,
                                                 const AlphabetTE   aAlphabet);

    static void             SDecode             (std::string_view   aBase58Str,
                                                 GpByteWriter&      aWriterData,
                                                 const AlphabetTE   aAlphabet);
    static std::string      SDecodeToStr        (std::string_view   aBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static GpBytesArray     SDecodeToByteArray  (std::string_view   aBase58Str,
                                                 const AlphabetTE   aAlphabet);

private:
    static const auto&      Alphabet            (GpBase58Alphabet   aAlphabet) noexcept
    {
        return sAlphabets[aAlphabet.ID()];
    }

    static size_t           SEncodedSize        (GpSpanPtrByteR aData);
    static std::any         SDecodePrecalc      (std::string_view   aBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static void             SDecode             (const std::any&    aDecodePrecalc,
                                                 GpByteWriter&      aWriterData);
    static size_t           SFindChId           (const u_int_8                  aCh,
                                                 const GpArray<u_int_8, 58>&    aAlphabet);

private:
    static GpArray<u_int_8, 58> sAlphabets[GpBase58Alphabet::SCount()];
};

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
