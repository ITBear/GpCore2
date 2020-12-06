#include "GpBitReader.hpp"
#include "../../Exceptions/GpException.hpp"

namespace GPlatform {

void    GpBitReader::Bits (GpRawPtrByteRW   aDataOut,
                           const size_bit_t aSize,
                           const size_bit_t aOffset)
{
    _Bits(aDataOut.Ptr(), aSize, aOffset);
}

u_int_8 GpBitReader::UInt8 (void)
{
    return ReadPOD<u_int_8>(size_byte_t::SMake(sizeof(u_int_8)), 0_bit);
}

u_int_8 GpBitReader::UInt8 (const size_bit_t aSize,
                            const size_bit_t aOffset)
{
    return ReadPOD<u_int_8>(aSize, aOffset);
}

s_int_8 GpBitReader::SInt8 (void)
{
    return ReadPOD<s_int_8>(size_byte_t::SMake(sizeof(s_int_8)), 0_bit);
}

s_int_8 GpBitReader::SInt8 (const size_bit_t aSize,
                            const size_bit_t aOffset)
{
    return ReadPOD<s_int_8>(aSize, aOffset);
}

u_int_16    GpBitReader::UInt16 (void)
{
    return ReadPOD<u_int_16>(size_byte_t::SMake(sizeof(u_int_16)), 0_bit);
}

u_int_16    GpBitReader::UInt16 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<u_int_16>(aSize, aOffset);
}

s_int_16    GpBitReader::SInt16 (void)
{
    return ReadPOD<s_int_16>(size_byte_t::SMake(sizeof(s_int_16)), 0_bit);
}

s_int_16    GpBitReader::SInt16 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<s_int_16>(aSize, aOffset);
}

u_int_32    GpBitReader::UInt32 (void)
{
    return ReadPOD<u_int_32>(size_byte_t::SMake(sizeof(u_int_32)), 0_bit);
}

u_int_32    GpBitReader::UInt32 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<u_int_32>(aSize, aOffset);
}

s_int_32    GpBitReader::SInt32 (void)
{
    return ReadPOD<s_int_32>(size_byte_t::SMake(sizeof(s_int_32)), 0_bit);
}

s_int_32    GpBitReader::SInt32 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<s_int_32>(aSize, aOffset);
}

u_int_64    GpBitReader::UInt64 (void)
{
    return ReadPOD<u_int_64>(size_byte_t::SMake(sizeof(u_int_64)), 0_bit);
}

u_int_64    GpBitReader::UInt64 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<u_int_64>(aSize, aOffset);
}

s_int_64    GpBitReader::SInt64 (void)
{
    return ReadPOD<s_int_64>(size_byte_t::SMake(sizeof(s_int_64)), 0_bit);
}

s_int_64    GpBitReader::SInt64 (const size_bit_t aSize,
                                 const size_bit_t aOffset)
{
    return ReadPOD<s_int_64>(aSize, aOffset);
}

void    GpBitReader::_Bits (std::byte*          aDataOut,
                            const size_bit_t    aSize,
                            const size_bit_t    aOffset)
{
    //Check range
    THROW_GPE_COND_CHECK_M(   (aSize > 0_bit)
                           && (iStorage.Left() >= aSize), "Out of range"_sv);

    //Copy
    std::byte*          dstPtr      = aDataOut;
    const size_bit_t    dstOffset   = aOffset;
    const std::byte*    srcPtr      = iStorage.Data();
    const size_bit_t    srcOffset   = iStorage.Size() - iStorage.Left();

    /*BitOps::Copy(dstPtr, dstOffset,
                 srcPtr, srcOffset,
                 aSize);*/

    Copy(dstPtr, dstOffset,
         srcPtr, srcOffset,
         aSize);

    //Shift
    iStorage.SetLeftSub(aSize);
}

void    GpBitReader::Copy (std::byte*       aDst,
                           const size_bit_t aDstOffset,
                           const std::byte* aSrc,
                           const size_bit_t aSrcOffset,
                           const size_bit_t aSize)
{
    const size_t size = aSize.As<size_t>();

    //Pointer to actual bytes
    aDst += aDstOffset.As<size_t>() / 8;
    const size_t dstOffset = aDstOffset.As<size_t>() % 8;

    aSrc += aSrcOffset.As<size_t>() / 8;
    const size_t srcOffset = aSrcOffset.As<size_t>() % 8;

    //Bytes intersection with ...offset[size...]
    size_t dstBytesIntersectCnt = NumOps::SAdd(size, dstOffset);
    size_t srcBytesIntersectCnt = NumOps::SAdd(size, srcOffset);

    if ((dstBytesIntersectCnt % 8) > 0)
    {
        dstBytesIntersectCnt = (dstBytesIntersectCnt / 8) + 1;
    } else
    {
        dstBytesIntersectCnt = (dstBytesIntersectCnt / 8);
    }

    if ((srcBytesIntersectCnt % 8) > 0)
    {
        srcBytesIntersectCnt = (srcBytesIntersectCnt / 8) + 1;
    } else
    {
        srcBytesIntersectCnt = (srcBytesIntersectCnt / 8);
    }

    //Check tmp buffers size
    using TmpBufferT = u_int_64;
    THROW_GPE_COND_CHECK_M(   (dstBytesIntersectCnt <= (sizeof(TmpBufferT)-1))
                           && (srcBytesIntersectCnt <= (sizeof(TmpBufferT)-1)),
                              "tmp buffer size is out of range"_sv);

    //Copy to tmp buffers
    TmpBufferT tmpDst = 0;
    TmpBufferT tmpSrc = 0;

    std::memcpy(reinterpret_cast<std::byte*>(&tmpDst),
                aDst,
                dstBytesIntersectCnt);

    std::memcpy(reinterpret_cast<std::byte*>(&tmpSrc),
                aSrc,
                srcBytesIntersectCnt);

    //std::cout << "tmpDst: " << StrOps::SFromBits({&tmpDst, 1_cnt}) << std::endl;
    //std::cout << "tmpSrc: " << StrOps::SFromBits({&tmpSrc, 1_cnt}) << std::endl;

    //Shift src tmp buffer
    const size_t srcShiftTail = (sizeof(TmpBufferT) * 8) - (srcOffset + size);
    const size_t dstShiftTail = (sizeof(TmpBufferT) * 8) - (dstOffset + size);
    u_int_64 mask = ((u_int_64(1) << size) - u_int_64(1)) << srcShiftTail;

    //std::cout << "mask:   " << StrOps::SFromBits({&mask, 1_cnt}) << std::endl;

    tmpDst = BitOps::N2H(tmpDst);
    tmpSrc = BitOps::N2H(tmpSrc);
    //mask = BitOps::N2H(mask);
    if (srcShiftTail >= dstShiftTail)
    {
        const size_t offset = (srcShiftTail - dstShiftTail);
        tmpSrc >>= offset;
        mask   >>= offset;

        //std::cout << "SH..." << offset << std::endl;
    } else
    {
        const size_t offset = (dstShiftTail - srcShiftTail);
        tmpSrc <<= offset;
        mask   <<= offset;

        //std::cout << "SH..." << offset << std::endl;
    }

    tmpDst  = BitOps::N2H(tmpDst);
    tmpSrc  = BitOps::N2H(tmpSrc);
    mask    = BitOps::N2H(mask);

    //std::cout << "tmpDst: " << StrOps::SFromBits({&tmpDst, 1_cnt}) << std::endl;
    //std::cout << "tmpSrc: " << StrOps::SFromBits({&tmpSrc, 1_cnt}) << std::endl;
    //std::cout << "mask:   " << StrOps::SFromBits({&mask, 1_cnt}) << std::endl;

    tmpDst = BitOps::SetByMask(tmpDst, tmpSrc, mask);

    //std::cout << "tmpDst: " << StrOps::SFromBits({&tmpDst, 1_cnt}) << std::endl;

    //Copy from dst tmp buffer
    std::memcpy(aDst,
                reinterpret_cast<std::byte*>(&tmpDst),
                dstBytesIntersectCnt);
}

}//GPlatform
