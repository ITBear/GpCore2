#include "GpFile.hpp"

#if defined(GP_POSIX)
    #include "GpFilePosix.hpp"
#elif defined(GP_OS_WINDOWS)
    #include "GpFileWindows.hpp"
#endif

namespace GPlatform {

GP_ENUM_IMPL(GpFileFlag)

void    GpFile::Open
(
    std::string_view    aName,
     const GpFileFlags  aFlags
)
{
    if (iHandler != HandlerT())
    {
        Close();
    }

    iHandler = GpFileImpl::SOpen(aName, aFlags);
}

void    GpFile::Close (void) noexcept
{
    if (iHandler == HandlerT())
    {
        return;
    }

    GpFileImpl::SClose(iHandler);
    iHandler = HandlerT();
}

void    GpFile::Flush (void)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    GpFileImpl::SFlush(iHandler);
}

size_byte_t GpFile::Size (void) const
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    return GpFileImpl::SSize(iHandler);
}

void    GpFile::GoToPos (const size_byte_t aPos)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    GpFileImpl::SGoToPos(iHandler, aPos);
}

void    GpFile::GoToStartPos (void)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    GpFileImpl::SGoToStartPos(iHandler);
}

size_byte_t GpFile::GoToEndPos (void)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    return GpFileImpl::SGoToEndPos(iHandler);
}

size_byte_t GpFile::CurrentPos (void) const
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    return GpFileImpl::SCurrentPos(iHandler);
}

void    GpFile::Write (GpSpanPtrByteR aData)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    return GpFileImpl::SWrite(iHandler, aData);
}

void    GpFile::Read (GpSpanPtrByteRW aData)
{
    THROW_COND_GP
    (
        iHandler != HandlerT(),
        "File is not opened"_sv
    );

    return GpFileImpl::SRead(iHandler, aData);
}

}//namespace GPlatform
