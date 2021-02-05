#include "GpBase64.hpp"

#if defined(GP_USE_BASE64)

namespace GPlatform {

const GpArray<std::byte, 64> GpBase64::sAlphabet =
{
    std::byte('A'),std::byte('B'),std::byte('C'),std::byte('D'),std::byte('E'),std::byte('F'),std::byte('G'),std::byte('H'),std::byte('I'),std::byte('J'),std::byte('K'),std::byte('L'),std::byte('M'),std::byte('N'),std::byte('O'),std::byte('P'),std::byte('Q'),std::byte('R'),std::byte('S'),std::byte('T'),std::byte('U'),std::byte('V'),std::byte('W'),std::byte('X'),std::byte('Y'),std::byte('Z'),
    std::byte('a'),std::byte('b'),std::byte('c'),std::byte('d'),std::byte('e'),std::byte('f'),std::byte('g'),std::byte('h'),std::byte('i'),std::byte('j'),std::byte('k'),std::byte('l'),std::byte('m'),std::byte('n'),std::byte('o'),std::byte('p'),std::byte('q'),std::byte('r'),std::byte('s'),std::byte('t'),std::byte('u'),std::byte('v'),std::byte('w'),std::byte('x'),std::byte('y'),std::byte('z'),
    std::byte('0'),std::byte('1'),std::byte('2'),std::byte('3'),std::byte('4'),std::byte('5'),std::byte('6'),std::byte('7'),std::byte('8'),std::byte('9'),std::byte('+'),std::byte('/')
};
/*
count_t GpBase64::Encode
(
    const void* _R_ aData,
    const size_byte_t   aDataSize,
    GpString&           aStrOut
)
{


    return Encode(aData,
                  aDataSize,
                  aStrOut.Elements() + oldCount.Value(),
                  charsCount);
}

count_t GpBase64::Encode (const void* _R_   aData,
                          const size_byte_t aDataSize,
                          std::string&      aStrOut)
{
    size_t base64BlocksCount = aDataSize.Value() / 3;
    if ((aDataSize.Value() % 3) > 0)
    {
        base64BlocksCount++;
    }

    const size_t charsCount = base64BlocksCount*4;
    const size_t oldCount   = aStrOut.size();

    aStrOut.resize(oldCount + charsCount);

    return Encode(aData,
                  aDataSize,
                  aStrOut.data() + oldCount,
                  count_t::MakeFromRaw(charsCount));
}

count_t GpBase64::Encode (const void* _R_   aData,
                          const size_byte_t aDataSize,
                          char*             aStrOut,
                          const count_t     aStrOutMaxSize)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //----------------- Check -------------------
    if (aData == nullptr)
    {
        return 0_cnt;
    }

    if (aDataSize == 0_byte)
    {
        return 0_cnt;
    }

    //----------------- Generate BASE64 -------------------
    size_t              dataBytesLeft   = aDataSize.Value();
    const u_int_8* _R_  data            = static_cast<const u_int_8*>(aData);
    size_t              blocksCount     = aDataSize.Value() / 3;
    if ((aDataSize.Value() % 3) > 0)
    {
        blocksCount++;
    }

    if (aStrOutMaxSize.Value() < (blocksCount*4))
    {
        THROW_GP_EXCEPTION("aStrOutMaxSize is too small"_sv);
    }

    size_t quarterId = 0;
    for (size_t blockId = 0; blockId < blocksCount; ++blockId)
    {
        size_t          blockData   = 0;
        const size_t    bytesToRead = GpMathUtils::Min(size_t(3), dataBytesLeft);
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
            *aStrOut++ = sAlphabet[charId];
        }
    }

    for ( ; quarterId < 4; ++quarterId)
    {
        *aStrOut++ = '=';
    }

    return count_t::MakeFromRaw(blocksCount*4);
}

size_byte_t GpBase64::Decode (std::string_view  aBase64Str,
                              GpBytesArray&     aDataOut)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check string length
    size_t strLength = aBase64Str.size();

    if (strLength == 0)
    {
        return 0_byte;
    }

    if (strLength < 4)
    {
        THROW_GP_EXCEPTION("String length musdt be >= 4"_sv);
    }

    //Check size
    if ((strLength % 4) > 0)
    {
        THROW_GP_EXCEPTION("String length must be multiple of 4"_sv);
    }

    const size_t  blocksCount   = strLength / 4;
    const size_t  maxDataOutSize= blocksCount*3;
    const count_t oldCount      = aDataOut.Count();

    aDataOut.ResizeAdd(count_t::MakeFromRaw(maxDataOutSize));

    const size_byte_t bytesRead = Decode(aBase64Str,
                                         aDataOut.Elements() + oldCount.Value(),
                                         size_byte_t::MakeFromRaw(maxDataOutSize));

    aDataOut.Resize(oldCount + count_t::MakeFromRaw(bytesRead.Value()));

    return bytesRead;
}

size_byte_t GpBase64::Decode (std::string_view  aBase64Str,
                              void* _R_         aDataOut,
                              const size_byte_t aDataOutMaxSize)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    size_t              dataSize    = aBase64Str.size();
    const u_int_8* _R_  base64Str   = reinterpret_cast<const u_int_8*>(aBase64Str.data());

    if (dataSize == 0)
    {
        return 0_byte;
    }

    if ((dataSize % 4) > 0)
    {
        THROW_GP_EXCEPTION("Data size must be multiple of 4"_sv);
    }

    const size_t blocksCount    = dataSize / 4;
    const size_t maxDataOutSize = blocksCount*3;

    if (aDataOutMaxSize.Value() < maxDataOutSize)
    {
        THROW_GP_EXCEPTION("aString.size() must be multiple of 4"_sv);
    }

    u_int_8* _R_        dataOut     = reinterpret_cast<u_int_8*>(aDataOut);
    size_t              bytesRead   = 0;

    for (size_t blockId = 0; blockId < blocksCount; ++blockId)
    {
        size_t  blockPartsRead  = 0;
        bool    isEnd           = false;

        u_int_32 blockData = u_int_32((size_t(AlphabetToByte(base64Str[0], blockPartsRead, isEnd)) << 18) |
                                      (size_t(AlphabetToByte(base64Str[1], blockPartsRead, isEnd)) << 12) |
                                      (size_t(AlphabetToByte(base64Str[2], blockPartsRead, isEnd)) <<  6) |
                                      (size_t(AlphabetToByte(base64Str[3], blockPartsRead, isEnd)) <<  0));

        base64Str += 4;

        const size_t blockBytesRead = blockPartsRead - 1;
        bytesRead += blockBytesRead;

        if (blockBytesRead == 3)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  8) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  0) & 0xFF);
        } else if (blockBytesRead == 2)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  8) & 0xFF);
        } else if (blockBytesRead == 1)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
        }
    }

    return size_byte_t::MakeFromRaw(bytesRead);
}*/

size_byte_t GpBase64::SEncode
(
    GpRawPtrByteR   aSrcData,
    GpByteWriter&   aWriterBase64Str,
    const count_t   aSingleLineMaxLength
)
{
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    //Check if empty
    if (aSrcData.IsEmpty())
    {
        return 0_byte;
    }

    //----------------- Generate BASE64 -------------------
    const size_t        blocksCount     = SCalcBlocksCount(aSrcData).As<size_t>();
    const size_byte_t   totalSize       = SCalcEncodedSize(aSrcData, aSingleLineMaxLength);
    size_t              dataBytesLeft   = aSrcData.CountLeft().As<size_t>();
    const u_int_8* _R_  data            = aSrcData.PtrAs<const u_int_8*>();

    GpRawPtrByteRW dataOut = aWriterBase64Str.OffsetAdd(totalSize).DataOut();
    dataOut.OffsetSub(totalSize.As<count_t>());

    std::byte* encodedStr = dataOut.Ptr();

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

    return totalSize;
}

std::string GpBase64::SEncode
(
    GpRawPtrByteR   aData,
    const count_t   aSingleLineMaxLength
)
{
    const size_byte_t   encodedSize = SCalcEncodedSize(aData, aSingleLineMaxLength);
    std::string         encodedStr;
    encodedStr.resize(encodedSize.As<size_t>());

    GpByteWriterStorageFixedSize    writerStaorge({encodedStr.data(), encodedStr.size()});
    GpByteWriter                    writer(writerStaorge);

    SEncode(aData, writer, aSingleLineMaxLength);

    return encodedStr;
}

size_byte_t GpBase64::SDecode
(
    GpRawPtrCharR   aBase64Str,
    GpByteWriter&   aWriterData
)
{
    THROW_NOT_IMPLEMENTED();


    /*
    static_assert(sizeof(size_t) >= 4, "sizeof(size_t) must be greater or equal to 4");

    size_t              dataSize    = aBase64Str.size();
    const u_int_8* _R_  base64Str   = reinterpret_cast<const u_int_8*>(aBase64Str.data());

    if (dataSize == 0)
    {
        return 0_byte;
    }

    if ((dataSize % 4) > 0)
    {
        THROW_GP_EXCEPTION("Data size must be multiple of 4"_sv);
    }

    const size_t blocksCount    = dataSize / 4;
    const size_t maxDataOutSize = blocksCount*3;

    if (aDataOutMaxSize.Value() < maxDataOutSize)
    {
        THROW_GP_EXCEPTION("aString.size() must be multiple of 4"_sv);
    }

    u_int_8* _R_        dataOut     = reinterpret_cast<u_int_8*>(aDataOut);
    size_t              bytesRead   = 0;

    for (size_t blockId = 0; blockId < blocksCount; ++blockId)
    {
        size_t  blockPartsRead  = 0;
        bool    isEnd           = false;

        u_int_32 blockData = u_int_32((size_t(AlphabetToByte(base64Str[0], blockPartsRead, isEnd)) << 18) |
                                      (size_t(AlphabetToByte(base64Str[1], blockPartsRead, isEnd)) << 12) |
                                      (size_t(AlphabetToByte(base64Str[2], blockPartsRead, isEnd)) <<  6) |
                                      (size_t(AlphabetToByte(base64Str[3], blockPartsRead, isEnd)) <<  0));

        base64Str += 4;

        const size_t blockBytesRead = blockPartsRead - 1;
        bytesRead += blockBytesRead;

        if (blockBytesRead == 3)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  8) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  0) & 0xFF);
        } else if (blockBytesRead == 2)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
            *dataOut++ = u_int_8((blockData >>  8) & 0xFF);
        } else if (blockBytesRead == 1)
        {
            *dataOut++ = u_int_8((blockData >> 16) & 0xFF);
        }
    }

    return size_byte_t::MakeFromRaw(bytesRead);
    */
}

std::string GpBase64::SDecode (GpRawPtrByteR aBase64Str)
{
    THROW_NOT_IMPLEMENTED();
    //?
}

size_byte_t GpBase64::SCalcEncodedSize
(
    GpRawPtrByteR   aSrcData,
    const count_t   aSingleLineMaxLength
)
{
    count_t base64BlocksCount   = SCalcBlocksCount(aSrcData);
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

count_t GpBase64::SCalcBlocksCount (GpRawPtrByteR aSrcData)
{
    const size_byte_t scrDataSize = aSrcData.SizeLeft();

    count_t base64BlocksCount = scrDataSize.As<count_t>() / 3_cnt;
    if ((scrDataSize.As<count_t>() % 3_cnt) > 0_cnt)
    {
        base64BlocksCount++;
    }

    return base64BlocksCount;
}

u_int_8 GpBase64::SAlphabetToByte
(
    const u_int_8   aValue,
    size_t&         aBlockPartsRead,
    bool&           aIsEnd
)
{
    u_int_8 v = 0;
    u_int_8 s = 0;

    if ((aValue >= u_int_8('A')) && (aValue <= u_int_8('Z')))
    {
        v = u_int_8('A');
        s = 0;
    } else if ((aValue >= u_int_8('a')) && (aValue <= u_int_8('z')))
    {
        v = u_int_8('a');
        s = 26;
    } else if ((aValue >= u_int_8('0')) && (aValue <= u_int_8('9')))
    {
        v = u_int_8('0');
        s = 26*2;
    } else if (aValue == u_int_8('+'))
    {
        v = u_int_8('+');
        s = 26*2 + 10;
    } else if (aValue == u_int_8('/'))
    {
        v = u_int_8('/');
        s = 26*2 + 11;
    } else if (aValue == u_int_8('='))
    {
        if (aBlockPartsRead >= 2)
        {
            aIsEnd = true;
            return 0;
        } else
        {
            THROW_GPE("Wrong character '"_sv + char(aValue) + "' in BASE64 sequence"_sv);
        }
    } else
    {
        THROW_GPE("Wrong character '"_sv + char(aValue) + "' in BASE64 sequence"_sv);
    }

    if (!aIsEnd)
    {
        aBlockPartsRead++;
        return u_int_8((aValue - v) + s);
    } else
    {
        THROW_GPE("Wrong character '"_sv + char(aValue) + "' in BASE64 sequence"_sv);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_BASE64)
