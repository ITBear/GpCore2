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

#if defined(GP_OS_LINUX)
#   define file_open open64
#   define file_lseek lseek64
#   define file_truncate ftruncate64
#elif defined(GP_OS_MACOS)
#   define file_open open
#   define file_lseek lseek
#   define file_truncate ftruncate
#else
#   error Unsupported OS
#endif//

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
    static inline void              STruncate               (GpFile::HandlerT   aHandler,
                                                             std::string_view   aFileName,
                                                             size_byte_t        aNewSize);
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

    static inline bool              SIsPathReadable         (std::string_view aPath);
    static inline bool              SIsPathWritable         (std::string_view aPath);
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
        fd = file_open
        (
            std::data(fileName),
            flags,
            0666
        );
    } else
    {
        fd = file_open
        (
            std::data(fileName),
            flags
        );
    }

    VERIFY
    (
        fd >= 0,
        [&fileName]()
        {
            return fmt::format
            (
                "Failed to open file '{}', error: {}",
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

    VERIFY
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

    VERIFY
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
    const auto res = file_lseek
    (
        aHandler,
        NumOps::SConvert<s_int_64>(aPos.Value()),
        SEEK_SET
    );

    VERIFY
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
    const auto res = file_lseek
    (
        aHandler,
        0,
        SEEK_END
    );

    VERIFY
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
    const auto res = file_lseek
    (
        aHandler,
        0,
        SEEK_CUR
    );

    VERIFY
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

void    GpFileImpl::STruncate
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName,
    size_byte_t         aNewSize
)
{
    VERIFY
    (
        file_truncate(aHandler, NumOps::SConvert<s_int_64>(aNewSize.Value())) != -1,
        [aFileName, aNewSize]()
        {
            return fmt::format
            (
                "Failed to set file '{}' size to {}, error: {}",
                aFileName,
                aNewSize.Value(),
                GpErrno::SGetAndClear()
            );
        }
    );
}

void    GpFileImpl::STruncateToCurrentPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    const size_byte_t currentPos = SCurrentPos(aHandler, aFileName);

    VERIFY
    (
        ftruncate(aHandler, NumOps::SConvert<s_int_64>(currentPos.Value())) != -1,
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

    VERIFY
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

    VERIFY
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

    VERIFY
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

    VERIFY
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

bool    GpFileImpl::SIsPathReadable (std::string_view aPath)
{
    std::string s{aPath};
    return access(s.c_str(), R_OK) == 0;
}

bool    GpFileImpl::SIsPathWritable (std::string_view aPath)
{
    std::string s{aPath};
    return access(s.c_str(), W_OK) == 0;
}

}// namespace GPlatform

#endif// #if defined(GP_POSIX)
