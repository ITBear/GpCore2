#include "GpBase64.hpp"

#if defined(GP_USE_BASE64)

#include "../Types/Strings/GpStringOps.hpp"
#include "../Streams/GpByteWriter.hpp"
#include "../Streams/GpByteWriterStorage.hpp"
#include "../Streams/GpByteWriterStorageFixedSize.hpp"

namespace GPlatform {

const std::array<u_int_8, 64> GpBase64::sAlphabet =
{
    u_int_8('A'),u_int_8('B'),u_int_8('C'),u_int_8('D'),u_int_8('E'),u_int_8('F'),u_int_8('G'),u_int_8('H'),u_int_8('I'),u_int_8('J'),u_int_8('K'),u_int_8('L'),u_int_8('M'),u_int_8('N'),u_int_8('O'),u_int_8('P'),u_int_8('Q'),u_int_8('R'),u_int_8('S'),u_int_8('T'),u_int_8('U'),u_int_8('V'),u_int_8('W'),u_int_8('X'),u_int_8('Y'),u_int_8('Z'),
    u_int_8('a'),u_int_8('b'),u_int_8('c'),u_int_8('d'),u_int_8('e'),u_int_8('f'),u_int_8('g'),u_int_8('h'),u_int_8('i'),u_int_8('j'),u_int_8('k'),u_int_8('l'),u_int_8('m'),u_int_8('n'),u_int_8('o'),u_int_8('p'),u_int_8('q'),u_int_8('r'),u_int_8('s'),u_int_8('t'),u_int_8('u'),u_int_8('v'),u_int_8('w'),u_int_8('x'),u_int_8('y'),u_int_8('z'),
    u_int_8('0'),u_int_8('1'),u_int_8('2'),u_int_8('3'),u_int_8('4'),u_int_8('5'),u_int_8('6'),u_int_8('7'),u_int_8('8'),u_int_8('9'),u_int_8('+'),u_int_8('/')
};

void    GpBase64::SEncode
(
    GpSpanByteR     aData,
    GpByteWriter&   aWriterBase64Str,
    const size_t    aSingleLineMaxLength
)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check if empty
    if (aData.Empty())
    {
        return;
    }

    //----------------- Generate BASE64 -------------------
    const size_t        blocksCount     = SEncodedBlocksCount(aData);
    const size_t        encodedSize     = SEncodedSize(aData, aSingleLineMaxLength);
    size_t              dataBytesLeft   = aData.Count();
    const u_int_8* _R_  data            = aData.PtrAs<const u_int_8*>();
    GpSpanByteRW        base64StrOut    = aWriterBase64Str.OffsetAdd(encodedSize);
    u_int_8*            encodedStr      = base64StrOut.PtrAs<u_int_8*>();

    THROW_COND_GP
    (
        encodedStr != nullptr,
        "encodedStr is null"_sv
    );

    size_t      quarterId       = 0;
    size_t      charsAddedCount = 0;
    const bool  splitToLines    = aSingleLineMaxLength > 0;

    for (size_t blockId = 0; blockId < blocksCount; ++blockId)
    {
        size_t          blockData   = 0;
        const size_t    bytesToRead = std::min(size_t{3}, dataBytesLeft);
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
                if ((charsAddedCount % aSingleLineMaxLength) == 0)
                {
                    *encodedStr++ = u_int_8('\r');
                    *encodedStr++ = u_int_8('\n');
                }
            }
        }
    }

    for ( ; quarterId < 4; ++quarterId)
    {
        *encodedStr++ = u_int_8('=');
    }
}

std::string GpBase64::SEncodeToStr
(
    GpSpanByteR     aData,
    const size_t    aSingleLineMaxLength
)
{
    const size_t    encodedSize = SEncodedSize(aData, aSingleLineMaxLength);
    std::string     encodedStr;
    encodedStr.resize(encodedSize);

    GpByteWriterStorageFixedSize    writerStorge({std::data(encodedStr), std::size(encodedStr)});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aSingleLineMaxLength);

    return encodedStr;
}

GpBytesArray    GpBase64::SEncodeToByteArray
(
    GpSpanByteR     aData,
    const size_t    aSingleLineMaxLength
)
{
    const size_t    encodedSize = SEncodedSize(aData, aSingleLineMaxLength);
    GpBytesArray    encodedData;
    encodedData.resize(encodedSize);

    GpByteWriterStorageFixedSize    writerStorge({std::data(encodedData), std::size(encodedData)});
    GpByteWriter                    writer(writerStorge);

    SEncode(aData, writer, aSingleLineMaxLength);

    return encodedData;
}

void    GpBase64::SDecode
(
    std::string_view    aBase64Str,
    GpByteWriter&       aWriterData
)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check if empty
    if (aBase64Str.empty())
    {
        return;
    }

    const size_t        base64StrSize   = std::size(aBase64Str);
    const u_int_8* _R_  base64StrPtr    = reinterpret_cast<const u_int_8*>(std::data(aBase64Str));
    const size_t        decodedSize     = SDecodedSize(aBase64Str);
    GpSpanByteRW        dataOut         = aWriterData.OffsetAdd(decodedSize);
    u_int_8* _R_        decodedData     = dataOut.PtrAs<u_int_8*>();

    THROW_COND_GP
    (
        decodedData != nullptr,
        "decodedData is nullptr"_sv
    );

    std::array<u_int_8, 4>  blockBase64;
    size_t                  blockId = 0;
    size_t                  padding = 0;

    for (size_t id = 0; id < base64StrSize; id++)
    {
        const u_int_8 ch = *base64StrPtr++;
        if (   ((ch >= u_int_8('A')) && (ch <= u_int_8('Z')))
            || ((ch >= u_int_8('a')) && (ch <= u_int_8('z')))
            || ((ch >= u_int_8('0')) && (ch <= u_int_8('9')))
            ||  (ch == u_int_8('+'))
            ||  (ch == u_int_8('/')))
        {
            std::data(blockBase64)[blockId] = ch;
            blockId++;
        } else if (ch == u_int_8('='))
        {
            std::data(blockBase64)[blockId] = ch;
            blockId++;
            padding++;
        } else
        {
            continue;
        }

        if (blockId >= std::size(blockBase64))
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
                *decodedData++ = u_int_8((blockData >> u_int_32(16)) & u_int_32(0xFF));
                *decodedData++ = u_int_8((blockData >> u_int_32( 8)) & u_int_32(0xFF));
                *decodedData++ = u_int_8((blockData >> u_int_32( 0)) & u_int_32(0xFF));
            } else if (padding == 1)
            {
                *decodedData++ = u_int_8((blockData >> u_int_32(16)) & u_int_32(0xFF));
                *decodedData++ = u_int_8((blockData >> u_int_32( 8)) & u_int_32(0xFF));
                break;
            } else if (padding == 2)
            {
                *decodedData++ = u_int_8((blockData >> u_int_32(16)) & u_int_32(0xFF));
                break;
            }

            blockId = 0;
            padding = 0;
        }
    }
}

std::string GpBase64::SDecodeToStr (std::string_view aBase64Str)
{
    const size_t    decodedSize = SDecodedSize(aBase64Str);
    std::string     decodedStr;
    decodedStr.resize(decodedSize);

    GpByteWriterStorageFixedSize    writerStorge({std::data(decodedStr), std::size(decodedStr)});
    GpByteWriter                    writer(writerStorge);

    SDecode(aBase64Str, writer);

    return decodedStr;
}

GpBytesArray    GpBase64::SDecodeToByteArray (std::string_view aBase64Str)
{
    const size_t    decodedSize = SDecodedSize(aBase64Str);
    GpBytesArray    decodedBytesArray;
    decodedBytesArray.resize(decodedSize);

    GpByteWriterStorageFixedSize    writerStorge(GpSpanByteRW(std::data(decodedBytesArray), std::size(decodedBytesArray)));
    GpByteWriter                    writer(writerStorge);

    SDecode(aBase64Str, writer);

    return decodedBytesArray;
}

size_t  GpBase64::SEncodedSize
(
    GpSpanByteR     aData,
    const size_t    aSingleLineMaxLength
)
{
    const size_t    base64BlocksCount   = SEncodedBlocksCount(aData);
    size_t          encodedSize         = NumOps::SMul(base64BlocksCount, size_t{4});

    if (aSingleLineMaxLength > 0)
    {
        size_t linesCount = encodedSize / aSingleLineMaxLength;

        if ((encodedSize % aSingleLineMaxLength) > 0)
        {
            linesCount = NumOps::SInc(linesCount);
        }

        encodedSize += NumOps::SMul(NumOps::SSub(linesCount, size_t{1}), size_t{2})/*\r\n*/;
    }

    return encodedSize;
}

size_t  GpBase64::SDecodedSize (std::string_view aBase64Str)
{
    const size_t base64Size = std::size(aBase64Str);

    if (base64Size == 0)
    {
        return 0;
    }

    const u_int_8* _R_  base64Str   = reinterpret_cast<const u_int_8*>(std::data(aBase64Str));
    size_t              decodedSize = 0;
    size_t              paddingSize = 0;

    for (size_t id = 0; id < base64Size; id++)
    {
        const u_int_8 ch = *base64Str++;
        if (   ((ch >= u_int_8('A')) && (ch <= u_int_8('Z')))
            || ((ch >= u_int_8('a')) && (ch <= u_int_8('z')))
            || ((ch >= u_int_8('0')) && (ch <= u_int_8('9')))
            || (ch == u_int_8('+'))
            || (ch == u_int_8('/')))
        {
            decodedSize++;
        } else if (ch == u_int_8('='))
        {
            paddingSize++;
        } else if (   (ch == u_int_8('\r'))
                   || (ch == u_int_8('\n'))
                   || (ch == u_int_8('\t'))
                   || (ch == u_int_8(' ' )))
        {
            //skip
        } else
        {
            THROW_GP("Wrong character '"_sv + std::bit_cast<char>(ch) + "'"_sv);
        }
    }

    THROW_COND_GP
    (
        ((decodedSize + paddingSize) % 4) == 0,
        "Base64 string size must be multiple of 4"_sv
    );

    if (paddingSize == 0)
    {
        return (decodedSize / 4) * 3;
    } else if (paddingSize == 2)
    {
        return (decodedSize / 4) * 3 + 1;
    } else if (paddingSize == 1)
    {
        return (decodedSize / 4) * 3 + 2;
    } else
    {
        THROW_GP("Wrong Base64 padding '=' count"_sv);
    }

    return decodedSize;
}

size_t  GpBase64::SEncodedBlocksCount (GpSpanByteR aData)
{
    const size_t dataSize = aData.SizeInBytes();

    size_t base64BlocksCount = dataSize / 3;
    if ((dataSize % 3) > 0)
    {
        base64BlocksCount++;
    }

    return base64BlocksCount;
}

u_int_8 GpBase64::SAlphabetToByte (const u_int_8 aValue)
{
    size_t v = 0;
    size_t s = 0;

    if ((aValue >= u_int_8('A')) && (aValue <= u_int_8('Z')))
    {
        v = size_t{'A'};
        s = 0;
    } else if ((aValue >= u_int_8('a')) && (aValue <= u_int_8('z')))
    {
        v = size_t{'a'};
        s = 26;
    } else if ((aValue >= u_int_8('0')) && (aValue <= u_int_8('9')))
    {
        v = size_t{'0'};
        s = 26*2;
    } else if (aValue == u_int_8('+'))
    {
        v = size_t{'+'};
        s = 26*2 + 10;
    } else if (aValue == u_int_8('/'))
    {
        v = size_t{'/'};
        s = 26*2 + 11;
    }

    return u_int_8((size_t(aValue) - v) + s);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_BASE64)
