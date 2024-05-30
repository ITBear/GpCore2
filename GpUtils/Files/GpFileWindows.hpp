#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_OS_WINDOWS)

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "GpFile.hpp"
#include "../Other/GpErrno.hpp"

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
        std::data(fname),
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
        [&aName]()
        {
            return fmt::format
            (
                "Failed to open file '{}', CreateFileA return error: {}",
                aName,
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

void    GpFileImpl::SFlush (GpFile::HandlerT aHandler)
{
    if (!aHandler)
    {
        return;
    }

    const auto res = FlushFileBuffers(aHandler);

    THROW_COND_GP
    (
        res,
        []()
        {
            return fmt::format("Failed to flush file data, error: {}", GpErrno::SWinGetAndClear());
        }
    );
}

size_byte_t GpFileImpl::SSize (GpFile::HandlerT aHandler)
{
    if (!aHandler)
    {
        return 0_byte;
    }

    const DWORD fileSize = GetFileSize(aHandler, nullptr);

    THROW_COND_GP
    (
        fileSize != INVALID_FILE_SIZE,
        []()
        {
            return fmt::format("Failed to get file size, error: {}", GpErrno::SWinGetAndClear());
        }
    );

    return size_byte_t::SMake(fileSize);
}

void    GpFileImpl::SGoToPos
(
    GpFile::HandlerT    aHandler,
    const size_byte_t   aPos
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
        [&aPos]()
        {
            return fmt::format("Failed to set file position (target pos = {}), error: {}", aPos.Value(), GpErrno::SWinGetAndClear());
        }
    );
}

void    GpFileImpl::SGoToStartPos (GpFile::HandlerT aHandler)
{
    SGoToPos(aHandler, 0_byte);
}

size_byte_t GpFileImpl::SGoToEndPos (GpFile::HandlerT aHandler)
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
        []()
        {
            return fmt::format("Failed to set file position to end, error: {}", GpErrno::SWinGetAndClear());
        }
    );

    return size_byte_t::SMake(newPos.QuadPart);
}

size_byte_t GpFileImpl::SCurrentPos (GpFile::HandlerT aHandler)
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
        []()
        {
            return fmt::format("Failed to get file position, error: {}", GpErrno::SWinGetAndClear());
        }
    );

    return size_byte_t::SMake(currentPos.QuadPart);
}

size_byte_t GpFileImpl::STryWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData
)
{
    const DWORD bytesToWrite    = NumOps::SConvert<DWORD>(aData.Size().Value());
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
        []()
        {
            return fmt::format("Failed to write to file, error: {}", GpErrno::SWinGetAndClear());
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(bytesWritten));
}

void    GpFileImpl::SWrite
(
    GpFile::HandlerT    aHandler,
    GpSpanByteR         aData
)
{
    const size_byte_t bytesWritten = STryWrite(aHandler, aData);

    THROW_COND_GP
    (
        bytesWritten == aData.Size(),
        [&aData, bytesWritten]()
        {
            return fmt::format("Failed to write to file, only {} bytes of {} was written", bytesWritten.Value(), aData.Size().Value());
        }
    );
}

size_byte_t GpFileImpl::STryRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData
)
{
    const DWORD bytesToRead = NumOps::SConvert<DWORD>(aData.Size().Value());
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
        []()
        {
            return fmt::format("Failed to read from file, error: {}", GpErrno::SWinGetAndClear());
        }
    );

    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(bytesRead));
}

void    GpFileImpl::SRead
(
    GpFile::HandlerT    aHandler,
    GpSpanByteRW        aData
)
{
    const size_byte_t bytesRead = STryRead(aHandler, aData);

    THROW_COND_GP
    (
        bytesRead == aData.Size(),
        [&aData, bytesRead]()
        {
            return fmt::format("Failed to read from file, only {} bytes of {} was read", bytesRead.Value(), aData.Size().Value());
        }
    );
}

}// namespace GPlatform

#endif// #if defined(GP_OS_WINDOWS)

