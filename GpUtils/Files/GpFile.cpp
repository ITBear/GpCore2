#include <GpCore2/GpUtils/Files/GpFile.hpp>

#if defined(GP_USE_FILE_UTILS)

#if defined(GP_POSIX)
    #include <GpCore2/GpUtils/Files/GpFilePosix.hpp>
#elif defined(GP_OS_WINDOWS)
    #include <GpCore2/GpUtils/Files/GpFileWindows.hpp>
#endif

namespace GPlatform {

GP_ENUM_IMPL(GpFileFlag)

GpFile::GpFile (void) noexcept
{
}

GpFile::~GpFile  (void) noexcept
{
    Close();
}

void    GpFile::Open
(
    std::string_view    aName,
    const GpFileFlags   aFlags
)
{
    Close();

    iHandler    = GpFileImpl::SOpen(aName, aFlags);
    iName       = aName;
    iFlags      = aFlags;
}

void    GpFile::Close (void) noexcept
{
    if (iHandler == HandlerT{})
    {
        return;
    }

    if (iFlags.Test(GpFileFlag::WRITE))
    {
        GpFileImpl::SFlush(iHandler, iName);
    }

    GpFileImpl::SClose(iHandler);
    iHandler = HandlerT{};
    iName.clear();
    iFlags.Clear();
}

void    GpFile::Flush (void)
{
    if (iHandler == HandlerT{})
    {
        return;
    }

    GpFileImpl::SFlush(iHandler, iName);
}

size_byte_t GpFile::Size (void) const
{
    return GpFileImpl::SSize(iHandler, iName);
}

void    GpFile::GoToPos (const size_byte_t aPos)
{
    GpFileImpl::SGoToPos(iHandler, aPos, iName);
}

void    GpFile::GoToStartPos (void)
{
    GpFileImpl::SGoToStartPos(iHandler, iName);
}

size_byte_t GpFile::GoToEndPos (void)
{
    return GpFileImpl::SGoToEndPos(iHandler, iName);
}

size_byte_t GpFile::CurrentPos (void) const
{
    return GpFileImpl::SCurrentPos(iHandler, iName);
}

void    GpFile::TruncateToCurrentPos (void)
{
    GpFileImpl::STruncateToCurrentPos(iHandler, iName);
}

void    GpFile::Resize (const size_byte_t aNewSize)
{
    GpFileImpl::STruncate(iHandler, iName, aNewSize);
}

void    GpFile::Write (GpSpanByteR aData)
{
    return GpFileImpl::SWrite(iHandler, aData, iName);
}

void    GpFile::Read (GpSpanByteRW aData)
{
    return GpFileImpl::SRead(iHandler, aData, iName);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_FILE_UTILS)
