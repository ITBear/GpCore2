#pragma once

#define __USE_FILE_OFFSET64

#include "../Types/GpTypes.hpp"

#if defined(GP_POSIX)

#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "GpFile.hpp"

namespace GPlatform {

class GpFileImpl
{
public:
    static inline GpFile::HandlerT  SOpen           (std::string_view   aName,
                                                     const GpFileFlags  aFlags);
    static inline void              SClose          (GpFile::HandlerT aHandler) noexcept;
    static inline void              SFlush          (GpFile::HandlerT aHandler);
    static inline size_byte_t       SSize           (GpFile::HandlerT aHandler);

    static inline void              SGoToPos        (GpFile::HandlerT   aHandler,
                                                     const size_byte_t  aPos);
    static inline void              SGoToStartPos   (GpFile::HandlerT aHandler);
    static inline size_byte_t       SGoToEndPos     (GpFile::HandlerT aHandler);
    static inline size_byte_t       SCurrentPos     (GpFile::HandlerT aHandler);

    static inline size_byte_t       STryWrite       (GpFile::HandlerT   aHandler,
                                                     GpSpanPtrByteR     aData);

    static inline void              SWrite          (GpFile::HandlerT   aHandler,
                                                     GpSpanPtrByteR     aData);

     static inline size_byte_t      STryRead        (GpFile::HandlerT   aHandler,
                                                     GpSpanPtrByteRW    aData);

    static inline void              SRead           (GpFile::HandlerT   aHandler,
                                                     GpSpanPtrByteRW    aData);
};

GpFile::HandlerT    GpFileImpl::SOpen
(
    std::string_view    aName,
    const GpFileFlags   aFlags
)
{
    std::string fname(aName);//fname must be 0-teminated string

    int flags = O_CLOEXEC;

    if (   (aFlags.Test(GpFileFlag::READ))
        && (aFlags.Test(GpFileFlag::WRITE)))
    {
        flags |= O_RDWR;
    } else if (aFlags.Test(GpFileFlag::READ))
    {
        flags |= O_RDONLY;
    } else if (aFlags.Test(GpFileFlag::WRITE))
    {
        flags |= O_WRONLY;
    }

    if (aFlags.Test(GpFileFlag::CREATE))
    {
        flags |= O_CREAT;

        if (aFlags.Test(GpFileFlag::DO_NOT_REWRITE_EXISTED))
        {
            flags |= O_EXCL;
        }
    }

    if (aFlags.Test(GpFileFlag::APPEND))
    {
        flags |= O_APPEND;
    }

    if (aFlags.Test(GpFileFlag::TRUNCATE))
    {
        flags |= O_TRUNC;
    }

    int fd = 0;

    if (aFlags.Test(GpFileFlag::CREATE))
    {
        fd = open64(fname.data(), flags, 0666);
    } else
    {
        fd = open64(fname.data(), flags);
    }

    THROW_COND_GP
    (
        fd >= 0,
        "Failed to open file '"_sv + aName + "', open64 return error code "_sv + errno
    );

    return fd;
}

void    GpFileImpl::SClose (GpFile::HandlerT aHandler) noexcept
{
    close(aHandler);
}

void    GpFileImpl::SFlush (GpFile::HandlerT aHandler)
{
    const int res = fsync(aHandler);

    THROW_COND_GP
    (
        res == 0,
        "Failed to flush data to file (fd = "_sv + aHandler + "), fsync return error code "_sv + errno
    );
}

size_byte_t GpFileImpl::SSize (GpFile::HandlerT aHandler)
{
    struct stat st;
    const auto res = fstat(aHandler, &st);

    THROW_COND_GP
    (
        res == 0,
        "Failed to get file info (fd = "_sv + aHandler + "), fstat return error code "_sv + errno
    );

    return size_byte_t::SMake(st.st_size);
}

void    GpFileImpl::SGoToPos
(
    GpFile::HandlerT    aHandler,
    const size_byte_t   aPos
)
{
    const auto res = lseek64
    (
        aHandler,
        NumOps::SConvert<__off64_t>(aPos.Value()),
        SEEK_SET
    );

    THROW_COND_GP
    (
        res != -1,
        "Failed to set file position (target pos = "_sv + aPos.Value() + ", fd = "_sv + aHandler + "), lseek64 return error code "_sv + errno
    );
}

void    GpFileImpl::SGoToStartPos (GpFile::HandlerT aHandler)
{
    SGoToPos(aHandler, 0_byte);
}

size_byte_t GpFileImpl::SGoToEndPos (GpFile::HandlerT aHandler)
{
    const auto res = lseek64
    (
        aHandler,
        0,
        SEEK_END
    );

    THROW_COND_GP
    (
        res != -1,
        "Failed to set file position (target pos = END, fd = "_sv + aHandler + "), lseek64 return error code "_sv + errno
    );

    return size_byte_t::SMake(res);
}

size_byte_t GpFileImpl::SCurrentPos (GpFile::HandlerT aHandler)
{
    const auto res = lseek64
    (
        aHandler,
        0,
        SEEK_CUR
    );

    THROW_COND_GP
    (
        res != -1,
        "Failed to set file position (target pos = CURRENT, fd = "_sv + aHandler + "), lseek64 return error code "_sv + errno
    );

    return size_byte_t::SMake(res);
}

size_byte_t GpFileImpl::STryWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanPtrByteR      aData
)
{
    const size_t sizeToWrite = NumOps::SConvert<size_t>(aData.Size().Value());

    const ssize_t res = write
    (
        aHandler,
        aData.Ptr(),
        sizeToWrite
    );

    if (res == -1)
    {
        THROW_GP("Failed to write to file (fd = "_sv + aHandler + "), write return error code "_sv + errno);
    }

    return size_byte_t::SMake(res);
}

void    GpFileImpl::SWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanPtrByteR      aData
)
{
    const size_byte_t sizeWritten = STryWrite(aHandler, aData);

    THROW_COND_GP
    (
        sizeWritten == aData.Size(),
        "Failed to write. Only "_sv + sizeWritten.Value() + " of "_sv + aData.Size().Value() + " bytes was written"_sv
    );
}

size_byte_t GpFileImpl::STryRead
(
    GpFile::HandlerT    aHandler,
    GpSpanPtrByteRW     aData
)
{
    const size_t sizeToRead = NumOps::SConvert<size_t>(aData.Size().Value());

    const ssize_t res = read
    (
        aHandler,
        aData.Ptr(),
        sizeToRead
    );

    if (res == -1)
    {
        THROW_GP("Failed to read from file (fd = "_sv + aHandler + "), read return error code "_sv + errno);
    }

    return size_byte_t::SMake(res);
}

void    GpFileImpl::SRead
(
    GpFile::HandlerT    aHandler,
    GpSpanPtrByteRW     aData
)
{
    const size_byte_t sizeRead = STryRead(aHandler, aData);

    THROW_COND_GP
    (
        sizeRead == aData.Size(),
        "Failed to read. Only "_sv + sizeRead.Value() + " of "_sv + aData.Size().Value() + " bytes was read"_sv
    );
}

}//namespace GPlatform

#endif//#if defined(GP_POSIX)
