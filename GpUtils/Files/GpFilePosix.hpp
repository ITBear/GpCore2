#pragma once

#define __USE_FILE_OFFSET64

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_POSIX)

#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <GpCore2/GpUtils/Files/GpFile.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>

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
                                                     GpSpanByteR        aData);

    static inline void              SWrite          (GpFile::HandlerT   aHandler,
                                                     GpSpanByteR        aData);

     static inline size_byte_t      STryRead        (GpFile::HandlerT   aHandler,
                                                     GpSpanByteRW       aData);

    static inline void              SRead           (GpFile::HandlerT   aHandler,
                                                     GpSpanByteRW       aData);
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

    GpFile::HandlerT fd = {};

    if (aFlags.Test(GpFileFlag::CREATE))
    {
        fd = open64
        (
            std::data(fname),
            flags,
            0666
        );
    } else
    {
        fd = open64
        (
            std::data(fname),
            flags
        );
    }

    THROW_COND_GP
    (
        fd >= 0,
        [&aName]()
        {
            return fmt::format
            (
                "Failed to open file '{}', open64 return error code {}",
                aName,
                GpErrno::SGetAndClear()
            );
        }
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
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to flush data to file (fd = {}), fsync return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
    );
}

size_byte_t GpFileImpl::SSize (GpFile::HandlerT aHandler)
{
    struct stat st;
    const auto res = fstat(aHandler, &st);

    THROW_COND_GP
    (
        res == 0,
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to get file info (fd = {}), fstat return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
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
        [&aPos, &aHandler]()
        {
            return fmt::format
            (
                "Failed to set file position (target pos = {}, fd = {}), lseek64 return error code {}",
                aPos.Value(),
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
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
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to set file position (target pos = END, fd = {}), lseek64 return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
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
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to set file position (target pos = CURRENT, fd = {}), lseek64 return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(res);
}

size_byte_t GpFileImpl::STryWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData
)
{
    const size_t sizeToWrite = aData.SizeInBytes();

    const ssize_t res = write
    (
        aHandler,
        aData.Ptr(),
        sizeToWrite
    );

    THROW_COND_GP
    (
        res != -1,
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to write to file (fd = {}), write return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(res));
}

void    GpFileImpl::SWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData
)
{
    const size_byte_t sizeWritten = STryWrite(aHandler, aData);

    THROW_COND_GP
    (
        sizeWritten == size_byte_t::SMake(aData.SizeInBytes()),
        [&sizeWritten, &aData]()
        {
            return fmt::format
            (
                "Failed to write. Only {} of {} bytes was written",
                sizeWritten.Value(),
                aData.SizeInBytes()
            );
        }
    );
}

size_byte_t GpFileImpl::STryRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData
)
{
    const size_t sizeToRead = NumOps::SConvert<size_t>(aData.SizeInBytes());

    const ssize_t res = read
    (
        aHandler,
        aData.Ptr(),
        sizeToRead
    );

    THROW_COND_GP
    (
        res != -1,
        [&aHandler]()
        {
            return fmt::format
            (
                "Failed to read from file (fd = {}), read return error code {}",
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(res));
}

void    GpFileImpl::SRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData
)
{
    const size_byte_t sizeRead = STryRead(aHandler, aData);

    THROW_COND_GP
    (
        sizeRead == size_byte_t::SMake(aData.SizeInBytes()),
        [&sizeRead, &aData]()
        {
            return fmt::format
            (
                "Failed to read. Only {} of {} bytes was read",
                sizeRead.Value(),
                aData.SizeInBytes()
            );
        }
    );
}

}// namespace GPlatform

#endif// #if defined(GP_POSIX)
