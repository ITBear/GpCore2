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
    static inline GpFile::HandlerT  SOpen                   (std::string_view   aFileName,
                                                             GpFileFlags        aFlags);
    static inline void              SClose                  (GpFile::HandlerT   aHandler) noexcept;
    static inline void              SFlush                  (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);
    static inline size_byte_t       SSize                   (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);

    static inline void              SGoToPos                (GpFile::HandlerT   aHandler,
                                                             size_byte_t        aPos,
                                                             std::string_view   aFileName);
    static inline void              SGoToStartPos           (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);
    static inline size_byte_t       SGoToEndPos             (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);
    static inline size_byte_t       SCurrentPos             (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);
    static inline void              STruncateToCurrentPos   (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName);

    static inline size_byte_t       STryWrite               (GpFile::HandlerT   aHandler,
                                                             GpSpanByteR        aData,
                                                             std::string_view   aFileName);

    static inline void              SWrite                  (GpFile::HandlerT   aHandler,
                                                             GpSpanByteR        aData,
                                                             std::string_view   aFileName);

     static inline size_byte_t      STryRead                (GpFile::HandlerT   aHandler,
                                                             GpSpanByteRW       aData,
                                                             std::string_view   aFileName);

    static inline void              SRead                   (GpFile::HandlerT   aHandler,
                                                             GpSpanByteRW       aData,
                                                             std::string_view   aFileName);
};

GpFile::HandlerT    GpFileImpl::SOpen
(
    std::string_view    aFileName,
    const GpFileFlags   aFlags
)
{
    const std::string fileName{aFileName};// fileName must be 0-teminated string

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
            std::data(fileName),
            flags,
            0666
        );
    } else
    {
        fd = open64
        (
            std::data(fileName),
            flags
        );
    }

    THROW_COND_GP
    (
        fd >= 0,
        [&fileName]()
        {
            return fmt::format
            (
                "Failed to open file '{}'. open64 returned error: {}",
                fileName,
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

void    GpFileImpl::SFlush
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    const int res = fsync(aHandler);

    THROW_COND_GP
    (
        res == 0,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to flush data for file '{}', error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );
}

size_byte_t GpFileImpl::SSize
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    struct stat st;
    const auto res = fstat(aHandler, &st);

    THROW_COND_GP
    (
        res == 0,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to get file '{}' size, error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(st.st_size);
}

void    GpFileImpl::SGoToPos
(
    GpFile::HandlerT    aHandler,
    const size_byte_t   aPos,
    std::string_view    aFileName
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
        [aPos, aFileName]()
        {
            return fmt::format
            (
                "Failed to set position for file '{}' (target position = {}), error: {}",
                aFileName,
                aPos.Value(),
                GpErrno::SGetAndClear()
            );
        }
    );
}

void    GpFileImpl::SGoToStartPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    SGoToPos(aHandler, 0_byte, aFileName);
}

size_byte_t GpFileImpl::SGoToEndPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
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
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to set the position of file '{}' to the end, error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(res);
}

size_byte_t GpFileImpl::SCurrentPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
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
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to get file '{}' position, error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(res);
}

void    GpFileImpl::STruncateToCurrentPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    const size_byte_t currentPos = SCurrentPos(aHandler, aFileName);

    THROW_COND_GP
    (
        ftruncate(aHandler, NumOps::SConvert<__off_t>(currentPos.Value())) != -1,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to set end of file '{}', error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );
}

size_byte_t GpFileImpl::STryWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData,
    std::string_view    aFileName
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
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to write to file '{}', error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(res));
}

void    GpFileImpl::SWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData,
    std::string_view    aFileName
)
{
    const size_byte_t sizeWritten = STryWrite(aHandler, aData, aFileName);

    THROW_COND_GP
    (
        sizeWritten == size_byte_t::SMake(aData.SizeInBytes()),
        [sizeWritten, aFileName, &aData]()
        {
            return fmt::format
            (
                "Failed to write to file '{}', only {} bytes out of {} were written",
                aFileName,
                sizeWritten.Value(),
                aData.SizeInBytes()
            );
        }
    );
}

size_byte_t GpFileImpl::STryRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData,
    std::string_view    aFileName
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
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to read from file '{}', error: {}",
                aFileName,
                GpErrno::SGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(res));
}

void    GpFileImpl::SRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData,
    std::string_view    aFileName
)
{
    const size_byte_t sizeRead = STryRead(aHandler, aData, aFileName);

    THROW_COND_GP
    (
        sizeRead == size_byte_t::SMake(aData.SizeInBytes()),
        [sizeRead, aFileName, &aData]()
        {
            return fmt::format
            (
                "Failed to read from file '{}', only {} bytes out of {} were read",
                aFileName,
                sizeRead.Value(),
                aData.SizeInBytes()
            );
        }
    );
}

}// namespace GPlatform

#endif// #if defined(GP_POSIX)
