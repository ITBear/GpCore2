#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_OS_WINDOWS)

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <GpCore2/GpUtils/Files/GpFile.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>

namespace GPlatform {

class GpFileImpl
{
public:
    static inline GpFile::HandlerT  SOpen                   (std::string_view   aName,
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
    const std::string fileName{aFileName};// fileName must be null-teminated string

    DWORD                   dwDesiredAccess         = {};
    DWORD                   dwShareMode             = FILE_SHARE_READ;
    LPSECURITY_ATTRIBUTES   lpSecurityAttributes    = {};
    DWORD                   dwCreationDisposition   = {};
    DWORD                   dwFlagsAndAttributes    = FILE_ATTRIBUTE_NORMAL;
    HANDLE                  hTemplateFile           = {};

    if (   (aFlags.Test(GpFileFlag::READ))
        && (aFlags.Test(GpFileFlag::WRITE)))
    {
        dwDesiredAccess |= GENERIC_READ;
        dwDesiredAccess |= GENERIC_WRITE;
    } else if (aFlags.Test(GpFileFlag::READ))
    {
        dwDesiredAccess |= GENERIC_READ;
    } else if (aFlags.Test(GpFileFlag::WRITE))
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if (aFlags.Test(GpFileFlag::CREATE))
    {
        if (aFlags.Test(GpFileFlag::DO_NOT_REWRITE_EXISTED))
        {
            dwCreationDisposition |= CREATE_NEW; // Creates a new file, fails if file already exists
        } else
        {
            dwCreationDisposition |= CREATE_ALWAYS; // Creates a new file, if file exists, it is overwritten and the existing attributes are cleared
        }
    } else
    {
        dwCreationDisposition |= OPEN_EXISTING;

        if (aFlags.Test(GpFileFlag::APPEND))
        {
            // NOP
        }

        if (aFlags.Test(GpFileFlag::TRUNCATE))
        {
            dwCreationDisposition |= TRUNCATE_EXISTING;
        }
    }

    GpFile::HandlerT fd = CreateFileA
    (
        std::data(fileName),
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile
    );

    THROW_COND_GP
    (
        fd != INVALID_HANDLE_VALUE,
        [&fileName]()
        {
            return fmt::format
            (
                "Failed to open file '{}'. CreateFileA returned error: {}",
                fileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return fd;
}

void    GpFileImpl::SClose (GpFile::HandlerT aHandler) noexcept
{
    if (!aHandler)
    {
        return;
    }

    CloseHandle(aHandler);
}

void    GpFileImpl::SFlush
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    if (!aHandler)
    {
        return;
    }

    const auto res = FlushFileBuffers(aHandler);

    THROW_COND_GP
    (
        res,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to flush data for file '{}', error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
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
    if (!aHandler)
    {
        return 0_byte;
    }

    const DWORD fileSize = GetFileSize(aHandler, nullptr);

    THROW_COND_GP
    (
        fileSize != INVALID_FILE_SIZE,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to get file '{}' size, error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(fileSize);
}

void    GpFileImpl::SGoToPos
(
    GpFile::HandlerT    aHandler,
    const size_byte_t   aPos,
    std::string_view    aFileName
)
{
    LARGE_INTEGER goToPos{.QuadPart = NumOps::SConvert<LONGLONG>(aPos.Value())};

    const auto res = SetFilePointerEx
    (
        aHandler,
        goToPos,
        nullptr,
        FILE_BEGIN
    );

    THROW_COND_GP
    (
        res,
        [aPos, aFileName]()
        {
            return fmt::format
            (
                "Failed to set position for file '{}' (target position = {}), error: {}",
                aFileName,
                aPos.Value(),
                GpErrno::SWinGetAndClear()
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
    LARGE_INTEGER newPos{.QuadPart = 0};
    const auto res = SetFilePointerEx
    (
        aHandler,
        LARGE_INTEGER{.QuadPart = 0},
        &newPos,
        FILE_END
    );

    THROW_COND_GP
    (
        res,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to set the position of file '{}' to the end, error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(newPos.QuadPart);
}

size_byte_t GpFileImpl::SCurrentPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    LARGE_INTEGER currentPos{.QuadPart = 0};
    const auto res = SetFilePointerEx
    (
        aHandler,
        LARGE_INTEGER{.QuadPart = 0},
        &currentPos,
        FILE_CURRENT
    );

    THROW_COND_GP
    (
        res,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to get file '{}' position, error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(currentPos.QuadPart);
}

void    GpFileImpl::STruncateToCurrentPos
(
    GpFile::HandlerT    aHandler,
    std::string_view    aFileName
)
{
    THROW_COND_GP
    (
        SetEndOfFile(aHandler) != 0,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to set end of file '{}', error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
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
    const DWORD bytesToWrite    = NumOps::SConvert<DWORD>(std::size(aData));
    DWORD       bytesWritten    = 0;

    const auto res = WriteFile
    (
        aHandler,
        aData.Ptr(),
        bytesToWrite,
        &bytesWritten,
        nullptr
    );

    THROW_COND_GP
    (
        res,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to write to file '{}', error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(bytesWritten));
}

void    GpFileImpl::SWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData,
    std::string_view    aFileName
)
{
    const size_byte_t bytesWritten = STryWrite(aHandler, aData, aFileName);

    THROW_COND_GP
    (
        bytesWritten.Value() == std::size(aData),
        [&aData, bytesWritten, aFileName]()
        {
            return fmt::format
            (
                "Failed to write to file '{}', only {} bytes out of {} were written",
                aFileName,
                bytesWritten.Value(),
                std::size(aData)
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
    const DWORD bytesToRead = NumOps::SConvert<DWORD>(std::size(aData));
    DWORD       bytesRead   = 0;

    const auto res = ReadFile
    (
        aHandler,
        aData.Ptr(),
        bytesToRead,
        &bytesRead,
        nullptr
    );

    THROW_COND_GP
    (
        res,
        [aFileName]()
        {
            return fmt::format
            (
                "Failed to read from file '{}', error: {}",
                aFileName,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(bytesRead));
}

void    GpFileImpl::SRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData,
    std::string_view    aFileName
)
{
    const size_byte_t bytesRead = STryRead(aHandler, aData, aFileName);

    THROW_COND_GP
    (
        bytesRead.Value() == std::size(aData),
        [&aData, bytesRead, aFileName]()
        {
            return fmt::format
            (
                "Failed to read from file '{}', only {} bytes out of {} were read",
                aFileName,
                bytesRead.Value(),
                std::size(aData)
            );
        }
    );
}

}// namespace GPlatform

#endif// #if defined(GP_OS_WINDOWS)

