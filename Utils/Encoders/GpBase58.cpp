#include "GpBase58.hpp"

#if defined(GP_USE_BASE58)

#if defined(GP_USE_GNU_GMP)
#   include <gmp/gmpxx.h>
#else
#   error please use GP_USE_GNU_GMP
#endif

namespace GPlatform {

GP_ENUM_IMPL(GpBase58Alphabet)

GpArray<u_int_8, 58>    GpBase58::sAlphabets[GpBase58Alphabet::SCount().As<size_t>()] =
{
    //NAKAMOTO
    {'1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','m','n','o','p','q','r','s','t','u','v','w','x','y','z'},

    //SHORT_URL
    {'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'}
};

void    GpBase58::SEncode
(
    GpRawPtrByteR       aData,
    GpByteWriter&       aWriterBase58Str,
    const AlphabetTE    aAlphabet
)
{
    THROW_GPE_COND(aData.CountLeft() > 0_cnt, "Data is empty");

    //----------------- Alphabet -------------------
    GpArray<u_int_8, 58> alphabet;
    std::memcpy(alphabet.data(), Alphabet(aAlphabet).data(), 58);

    //----------------- Generate BASE58 -------------------
    const size_byte_t encodedSize = SEncodedSize(aData);

    GpRawPtrByteRW base64StrOut = aWriterBase58Str.OffsetAdd(encodedSize).DataOut();
    base64StrOut.OffsetSub(encodedSize.As<count_t>());

    u_int_8* _R_ encodedStr = base64StrOut.PtrAs<u_int_8*>() + encodedSize.As<size_t>() - 1;

    mpz_class data;
    mpz_class remainder;
    mpz_class quotient;

    mpz_import(data.get_mpz_t(), aData.SizeLeft().As<size_t>(), 1, 1, 0, 0, aData.Ptr());

    while (data > 0)
    {
        mpz_divmod_ui
        (
            quotient.get_mpz_t(),
            remainder.get_mpz_t(),
            data.get_mpz_t(),
            58
        );

        *encodedStr-- = alphabet[remainder.get_ui()];
        data = quotient;
    }
}

std::string GpBase58::SEncodeToStr
(
    GpRawPtrByteR       aData,
    const AlphabetTE    aAlphabet
)
{
    const size_byte_t   encodedSize = SEncodedSize(aData);
    std::string         encodedStr;
    encodedStr.resize(encodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge({encodedStr.data(), encodedStr.size()});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aAlphabet);

    return encodedStr;
}

GpBytesArray    GpBase58::SEncodeToByteArray
(
    GpRawPtrByteR       aData,
    const AlphabetTE    aAlphabet
)
{
    const size_byte_t   encodedSize = SEncodedSize(aData);
    GpBytesArray        encodedData;
    encodedData.resize(encodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge({encodedData.data(), encodedData.size()});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aAlphabet);

    return encodedData;
}

void    GpBase58::SDecode
(
    GpRawPtrCharR       aBase58Str,
    GpByteWriter&       aWriterData,
    const AlphabetTE    aAlphabet
)
{
    const auto  decodedAny  = SDecodePrecalc(aBase58Str, aAlphabet);
    const auto& decodedInfo = std::any_cast<const std::tuple<mpz_class, size_t>&>(decodedAny);

    SDecode(decodedInfo, aWriterData);
}

std::string GpBase58::SDecodeToStr
(
    GpRawPtrCharR       aBase58Str,
    const AlphabetTE    aAlphabet
)
{
    const auto      decodedAny  = SDecodePrecalc(aBase58Str, aAlphabet);
    const auto&     decodedInfo = std::any_cast<const std::tuple<mpz_class, size_t>&>(decodedAny);
    const size_t    decodedSize = std::get<1>(decodedInfo);
    std::string     decodedStr;

    decodedStr.resize(decodedSize);

    GpByteWriterStorageFixedSize    writerStorge({decodedStr.data(), decodedStr.size()});
    GpByteWriter                    writer(writerStorge);

    SDecode(decodedInfo, writer);

    return decodedStr;
}

GpBytesArray    GpBase58::SDecodeToByteArray
(
    GpRawPtrCharR       aBase58Str,
    const AlphabetTE    aAlphabet
)
{
    const auto      decodedAny  = SDecodePrecalc(aBase58Str, aAlphabet);
    const auto&     decodedInfo = std::any_cast<const std::tuple<mpz_class, size_t>&>(decodedAny);
    const size_t    decodedSize = std::get<1>(decodedInfo);
    GpBytesArray    decodedData;

    decodedData.resize(decodedSize);

    GpByteWriterStorageFixedSize    writerStorge({decodedData.data(), decodedData.size()});
    GpByteWriter                    writer(writerStorge);

    SDecode(decodedInfo, writer);

    return decodedData;
}

size_byte_t GpBase58::SEncodedSize (GpRawPtrByteR aData)
{
    const size_t dataSize = aData.CountLeft().As<size_t>();

    THROW_GPE_COND(dataSize > 0, "Data is empty");

    mpz_class data;
    mpz_class remainder;
    mpz_class quotient;

    mpz_import(data.get_mpz_t(), dataSize, 1, 1, 0, 0, aData.Ptr());

    size_byte_t encodedSize = 0_byte;

    //TODO: reimplement with log58(data)

    while (data > 0)
    {
        mpz_divmod_ui
        (
            quotient.get_mpz_t(),
            remainder.get_mpz_t(),
            data.get_mpz_t(),
            58
        );

        encodedSize++;
        data = quotient;
    }

    return encodedSize;
}

std::any    GpBase58::SDecodePrecalc
(
    GpRawPtrCharR       aBase58Str,
    const AlphabetTE    aAlphabet
)
{
    THROW_GPE_COND(aBase58Str.CountLeft() > 0_cnt, "Data is empty");

    //----------------- Alphabet -------------------
    GpArray<u_int_8, 58> alphabet;
    std::memcpy(alphabet.data(), Alphabet(aAlphabet).data(), 58);

    //----------------- Decode BASE58 -------------------
    mpz_class           data = 0;
    const u_int_8* _R_  base58StrPtr    = aBase58Str.PtrAs<const u_int_8*>();
    const size_t        base58StrSize   = aBase58Str.CountLeft().As<size_t>();

    for (size_t id = 0; id < base58StrSize; ++id)
    {
        const u_int_8   ch      = *base58StrPtr++;
        const size_t    chId    = SFindChId(ch, alphabet);

        data = data*size_t(58) + size_t(chId);
    }

    const size_t dataSizeInBase = mpz_sizeinbase(data.get_mpz_t(), 2);
    const size_t dataSize       = NumOps::SDiv<size_t>(NumOps::SAdd<size_t>(dataSizeInBase, (8-1)), 8);

    return std::make_tuple(data, dataSize);
}

void    GpBase58::SDecode
(
    const std::any& aDecodePrecalc,
    GpByteWriter&   aWriterData
)
{
    const auto&         decodedInfo = std::any_cast<const std::tuple<mpz_class, size_t>&>(aDecodePrecalc);
    const mpz_class&    bigNum      = std::get<0>(decodedInfo);
    size_t              decodedSize = std::get<1>(decodedInfo);

    GpRawPtrByteRW dataOut = aWriterData.OffsetAdd(size_byte_t::SMake(decodedSize)).DataOut();
    dataOut.OffsetSub(count_t::SMake(decodedSize));

    mpz_export(dataOut.Ptr(), &decodedSize, 1, 1, 0, 0, bigNum.get_mpz_t());
}

size_t  GpBase58::SFindChId
(
    const u_int_8               aCh,
    const GpArray<u_int_8, 58>& aAlphabet
)
{
    size_t id = 0;

    for (const u_int_8 a: aAlphabet)
    {
        if (a == aCh)
        {
            return id;
        }

        id++;
    }

    THROW_GPE("Wrong Base58 character '"_sv + aCh + "'"_sv);
}

}//GPlatform

#endif//#if defined(GP_USE_BASE58)
