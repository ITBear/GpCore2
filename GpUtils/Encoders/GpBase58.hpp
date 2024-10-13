#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_BASE58)

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

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
    static void             SEncode             (GpSpanByteR        aData,
                                                 GpByteWriter&      aWriterBase58Str,
                                                 const AlphabetTE   aAlphabet);
    static std::string      SEncodeToStr        (GpSpanByteR        aData,
                                                 const AlphabetTE   aAlphabet);
    static GpBytesArray     SEncodeToByteArray  (GpSpanByteR        aData,
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

    static size_t           SEncodedSize        (GpSpanByteR aData);
    static void             SDecodePrecalc      (std::string_view   aBase58Str,
                                                 const AlphabetTE   aAlphabet,
                                                 void*              aMpzClass,
                                                 size_t&            aDataSizeOut);
    static void             SDecode             (const void*        aMpzClass,
                                                 const size_t       aDataSize,
                                                 GpByteWriter&      aWriterData);
    static size_t           SFindChId           (const u_int_8                  aCh,
                                                 const std::array<u_int_8, 58>& aAlphabet);

private:
    static std::array<u_int_8, 58>  sAlphabets[GpBase58Alphabet::SCount()];
};

}// namespace GPlatform

#endif// #if defined(GP_USE_BASE64)
