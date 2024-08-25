#include <GpCore2/GpUtils/Files/GpFile.hpp>

#if defined(GP_USE_FILE_UTILS)

#if defined(GP_POSIX)
    #include <GpCore2/GpUtils/Files/GpFilePosix.hpp>
#elif defined(GP_OS_WINDOWS)
    #include <GpCore2/GpUtils/Files/GpFileWindows.hpp>
#endif

namespace GPlatform {

GP_ENUM_IMPL(GpFileFlag)

void    GpFile::Open
(
    std::string_view    aName,
    const GpFileFlags   aFlags
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
    GpFileImpl::SFlush(iHandler);
}

size_byte_t GpFile::Size (void) const
{
    return GpFileImpl::SSize(iHandler);
}

void    GpFile::GoToPos (const size_byte_t aPos)
{
    GpFileImpl::SGoToPos(iHandler, aPos);
}

void    GpFile::GoToStartPos (void)
{
    GpFileImpl::SGoToStartPos(iHandler);
}

size_byte_t GpFile::GoToEndPos (void)
{
    return GpFileImpl::SGoToEndPos(iHandler);
}

size_byte_t GpFile::CurrentPos (void) const
{
    return GpFileImpl::SCurrentPos(iHandler);
}

void    GpFile::Write (GpSpanByteR aData)
{
    return GpFileImpl::SWrite(iHandler, aData);
}

void    GpFile::Read (GpSpanByteRW aData)
{
    return GpFileImpl::SRead(iHandler, aData);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_FILE_UTILS)
