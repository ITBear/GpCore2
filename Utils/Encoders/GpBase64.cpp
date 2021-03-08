#include "GpBase64.hpp"

#if defined(GP_USE_BASE64)

namespace GPlatform {

const GpArray<std::byte, 64> GpBase64::sAlphabet =
{
    std::byte('A'),std::byte('B'),std::byte('C'),std::byte('D'),std::byte('E'),std::byte('F'),std::byte('G'),std::byte('H'),std::byte('I'),std::byte('J'),std::byte('K'),std::byte('L'),std::byte('M'),std::byte('N'),std::byte('O'),std::byte('P'),std::byte('Q'),std::byte('R'),std::byte('S'),std::byte('T'),std::byte('U'),std::byte('V'),std::byte('W'),std::byte('X'),std::byte('Y'),std::byte('Z'),
    std::byte('a'),std::byte('b'),std::byte('c'),std::byte('d'),std::byte('e'),std::byte('f'),std::byte('g'),std::byte('h'),std::byte('i'),std::byte('j'),std::byte('k'),std::byte('l'),std::byte('m'),std::byte('n'),std::byte('o'),std::byte('p'),std::byte('q'),std::byte('r'),std::byte('s'),std::byte('t'),std::byte('u'),std::byte('v'),std::byte('w'),std::byte('x'),std::byte('y'),std::byte('z'),
    std::byte('0'),std::byte('1'),std::byte('2'),std::byte('3'),std::byte('4'),std::byte('5'),std::byte('6'),std::byte('7'),std::byte('8'),std::byte('9'),std::byte('+'),std::byte('/')
};

void    GpBase64::SEncode
(
    GpRawPtrByteR   aData,
    GpByteWriter&   aWriterBase64Str,
    const count_t   aSingleLineMaxLength
)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check if empty
    if (aData.IsEmpty())
    {
        return;
    }

    //----------------- Generate BASE64 -------------------
    const size_t        blocksCount     = SEncodedBlocksCount(aData).As<size_t>();
    const size_byte_t   encodedSize     = SEncodedSize(aData, aSingleLineMaxLength);
    size_t              dataBytesLeft   = aData.CountLeft().As<size_t>();
    const u_int_8* _R_  data            = aData.PtrAs<const u_int_8*>();

    GpRawPtrByteRW base64StrOut = aWriterBase64Str.OffsetAdd(encodedSize).DataOut();
    base64StrOut.OffsetSub(encodedSize.As<count_t>());

    std::byte* encodedStr = base64StrOut.Ptr();

    size_t      quarterId       = 0;
    size_t      charsAddedCount = 0;
    const bool  splitToLines    = aSingleLineMaxLength > 0_cnt;

    for (size_t blockId = 0; blockId < blocksCount; ++blockId)
    {
        size_t          blockData   = 0;
        const size_t    bytesToRead = std::min(size_t(3), dataBytesLeft);
        size_t          shift       = 16;

        dataBytesLeft -= bytesToRead;

        for (size_t byteId = 0; byteId < bytesToRead; ++byteId)
        {
            blockData |= size_t(*data++) << shift;
            shift -= 8;
        }

        shift = 18;

        const size_t quartersCount = bytesToRead + 1;
        for (quarterId = 0; quarterId < quartersCount; ++quarterId)
        {
            const size_t charId = (blockData >> shift) & 0b00111111;
            shift -= 6;

            charsAddedCount++;
            *encodedStr++ = sAlphabet[charId];

            if (splitToLines)
            {
                if ((charsAddedCount % aSingleLineMaxLength.As<size_t>()) == 0)
                {
                    *encodedStr++ = std::byte('\r');
                    *encodedStr++ = std::byte('\n');
                }
            }
        }
    }

    for ( ; quarterId < 4; ++quarterId)
    {
        *encodedStr++ = std::byte('=');
    }
}

std::string GpBase64::SEncodeToStr
(
    GpRawPtrByteR   aData,
    const count_t   aSingleLineMaxLength
)
{
    const size_byte_t   encodedSize = SEncodedSize(aData, aSingleLineMaxLength);
    std::string         encodedStr;
    encodedStr.resize(encodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge({encodedStr.data(), encodedStr.size()});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aSingleLineMaxLength);

    return encodedStr;
}

GpBytesArray    GpBase64::SEncodeToByteArray
(
    GpRawPtrByteR   aData,
    const count_t   aSingleLineMaxLength
)
{
    const size_byte_t   encodedSize = SEncodedSize(aData, aSingleLineMaxLength);
    GpBytesArray        encodedData;
    encodedData.resize(encodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge({encodedData.data(), encodedData.size()});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aSingleLineMaxLength);

    return encodedData;
}

void    GpBase64::SDecode
(
    GpRawPtrCharR   aBase64Str,
    GpByteWriter&   aWriterData
)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check if empty
    if (aBase64Str.IsEmpty())
    {
        return;
    }

    const size_t            base64Size  = aBase64Str.SizeLeft().As<size_t>();
    const std::byte* _R_    base64Str   = aBase64Str.PtrAs<const std::byte*>();
    const size_byte_t       decodedSize = SDecodedSize(aBase64Str);

    GpRawPtrByteRW dataOut = aWriterData.OffsetAdd(decodedSize).DataOut();
    dataOut.OffsetSub(decodedSize.As<count_t>());

    std::byte* decodedData = dataOut.Ptr();

    GpArray<std::byte, 4>   blockBase64;
    size_t                  blockId = 0;
    size_t                  padding = 0;

    for (size_t id = 0; id < base64Size; id++)
    {
        const std::byte ch = *base64Str++;
        if (   ((ch >= std::byte('A')) && (ch <= std::byte('Z')))
            || ((ch >= std::byte('a')) && (ch <= std::byte('z')))
            || ((ch >= std::byte('0')) && (ch <= std::byte('9')))
            ||  (ch == std::byte('+'))
            ||  (ch == std::byte('/')))
        {
            blockBase64.data()[blockId] = ch;
            blockId++;
        } else if (ch == std::byte('='))
        {
            blockBase64.data()[blockId] = ch;
            blockId++;
            padding++;
        } else
        {
            continue;
        }

        if (blockId >= blockBase64.size())
        {
            u_int_32 blockData =
            (
                u_int_32(SAlphabetToByte(blockBase64[0])) << u_int_32(18) |
                u_int_32(SAlphabetToByte(blockBase64[1])) << u_int_32(12) |
                u_int_32(SAlphabetToByte(blockBase64[2])) << u_int_32( 6) |
                u_int_32(SAlphabetToByte(blockBase64[3])) << u_int_32( 0)
            );

            if (padding == 0)
            {
                *decodedData++ = std::byte((blockData >> u_int_32(16)) & u_int_32(0xFF));
                *decodedData++ = std::byte((blockData >> u_int_32( 8)) & u_int_32(0xFF));
                *decodedData++ = std::byte((blockData >> u_int_32( 0)) & u_int_32(0xFF));
            } else if (padding == 1)
            {
                *decodedData++ = std::byte((blockData >> u_int_32(16)) & u_int_32(0xFF));
                *decodedData++ = std::byte((blockData >> u_int_32( 8)) & u_int_32(0xFF));
                break;
            } else if (padding == 2)
            {
                *decodedData++ = std::byte((blockData >> u_int_32(16)) & u_int_32(0xFF));
                break;
            }

            blockId = 0;
            padding = 0;
        }
    }
}

std::string GpBase64::SDecodeToStr (GpRawPtrCharR aBase64Str)
{
    const size_byte_t   decodedSize = SDecodedSize(aBase64Str);
    std::string         decodedStr;
    decodedStr.resize(decodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge({decodedStr.data(), decodedStr.size()});
    GpByteWriter                    writer(writerStorge);

    SDecode(aBase64Str, writer);

    return decodedStr;
}

GpBytesArray    GpBase64::SDecodeToByteArray (GpRawPtrCharR aBase64Str)
{
    const size_byte_t   decodedSize = SDecodedSize(aBase64Str);
    GpBytesArray        decodedBytesArray;
    decodedBytesArray.resize(decodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStorge(decodedBytesArray);
    GpByteWriter                    writer(writerStorge);

    SDecode(aBase64Str, writer);

    return decodedBytesArray;
}

size_byte_t GpBase64::SEncodedSize
(
    GpRawPtrByteR   aData,
    const count_t   aSingleLineMaxLength
)
{
    count_t base64BlocksCount   = SEncodedBlocksCount(aData);
    count_t encodedSize         = base64BlocksCount * 4_cnt;

    if (aSingleLineMaxLength > 0_cnt)
    {
        count_t linesCount = encodedSize / aSingleLineMaxLength;

        if ((encodedSize % aSingleLineMaxLength) > 0_cnt)
        {
            linesCount++;
        }

        encodedSize += (linesCount - 1_cnt) * 2_cnt/*\r\n*/;
    }

    return encodedSize.As<size_byte_t>();
}

size_byte_t GpBase64::SDecodedSize (GpRawPtrCharR aBase64Str)
{
    const size_t base64Size = aBase64Str.SizeLeft().As<size_t>();

    if (base64Size == 0)
    {
        return 0_byte;
    }

    const std::byte* _R_    base64Str   = aBase64Str.PtrAs<const std::byte*>();
    size_byte_t             decodedSize = 0_byte;
    size_byte_t             paddingSize = 0_byte;

    for (size_t id = 0; id < base64Size; id++)
    {
        const std::byte ch = *base64Str++;
        if (   ((ch >= std::byte('A')) && (ch <= std::byte('Z')))
            || ((ch >= std::byte('a')) && (ch <= std::byte('z')))
            || ((ch >= std::byte('0')) && (ch <= std::byte('9')))
            || (ch == std::byte('+'))
            || (ch == std::byte('/')))
        {
            decodedSize++;
        } else if (ch == std::byte('='))
        {
            paddingSize++;
        } else if (   (ch == std::byte('\r'))
                   || (ch == std::byte('\n'))
                   || (ch == std::byte('\t'))
                   || (ch == std::byte(' ' )))
        {
            //skip
        } else
        {
            THROW_GPE("Wrong character '"_sv + std::string_view(reinterpret_cast<const char*>(&ch), 1) + "'"_sv);
        }
    }

    THROW_GPE_COND
    (
        ((decodedSize + paddingSize) % 4_byte) == 0_byte,
        "Base64 string size must be multiple of 4"_sv
    );

    if (paddingSize == 0_byte)
    {
        return (decodedSize / 4_byte) * 3_byte;
    } else if (paddingSize == 2_byte)
    {
        return (decodedSize / 4_byte) * 3_byte + 1_byte;
    } else if (paddingSize == 1_byte)
    {
        return (decodedSize / 4_byte) * 3_byte + 2_byte;
    } else
    {
        THROW_GPE("Wrong Base64 padding '=' count"_sv);
    }

    return decodedSize;
}

count_t GpBase64::SEncodedBlocksCount (GpRawPtrByteR aData)
{
    const size_byte_t scrDataSize = aData.SizeLeft();

    count_t base64BlocksCount = scrDataSize.As<count_t>() / 3_cnt;
    if ((scrDataSize.As<count_t>() % 3_cnt) > 0_cnt)
    {
        base64BlocksCount++;
    }

    return base64BlocksCount;
}

std::byte   GpBase64::SAlphabetToByte (const std::byte aValue)
{
    size_t v = 0;
    size_t s = 0;

    if ((aValue >= std::byte('A')) && (aValue <= std::byte('Z')))
    {
        v = size_t('A');
        s = 0;
    } else if ((aValue >= std::byte('a')) && (aValue <= std::byte('z')))
    {
        v = size_t('a');
        s = 26;
    } else if ((aValue >= std::byte('0')) && (aValue <= std::byte('9')))
    {
        v = size_t('0');
        s = 26*2;
    } else if (aValue == std::byte('+'))
    {
        v = size_t('+');
        s = 26*2 + 10;
    } else if (aValue == std::byte('/'))
    {
        v = size_t('/');
        s = 26*2 + 11;
    }

    return std::byte((size_t(aValue) - v) + s);
}

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
