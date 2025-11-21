#include <GpCore2/GpUtils/Files/GpFileUtils.hpp>
#include <GpCore2/GpUtils/Files/GpFile.hpp>

#if defined(GP_USE_FILE_UTILS)

#if defined(GP_POSIX)
    #include <GpCore2/GpUtils/Files/GpFilePosix.hpp>
#elif defined(GP_OS_WINDOWS)
    #include <GpCore2/GpUtils/Files/GpFileWindows.hpp>
#endif

#include <filesystem>

namespace GPlatform {

GpByteArray GpFileUtils::SReadAll (std::string_view aFileName)
{
    GpFile  file;
    file.Open(aFileName, {GpFileFlag::READ});
    const size_byte_t fileSize = file.Size();
    GpByteArray data;
    data.resize(fileSize.Value());
    file.Read(data);
    file.Close();

    return data;
}

void    GpFileUtils::SWriteAll
(
    std::string_view    aFileName,
    GpSpanByteR         aData
)
{
    GpFile  file;
    file.Open(aFileName, {GpFileFlag::WRITE | GpFileFlag::CREATE | GpFileFlag::TRUNCATE});
    file.Write(aData);
    file.Close();
}

void    GpFileUtils::SAppend
(
    const std::string_view  aFileName,
    const GpSpanByteR       aData
)
{
    GpFile  file;
    file.Open(aFileName, {GpFileFlag::WRITE | GpFileFlag::CREATE | GpFileFlag::APPEND});
    file.GoToEndPos();
    file.Write(aData);
    file.Close();
}

void    GpFileUtils::SCopy
(
    const std::string_view aFrom,
    const std::string_view aTo
)
{
    std::filesystem::copy(aFrom, aTo);
}

bool    GpFileUtils::SIsExists (std::string_view aFileName)
{
    return std::filesystem::exists(aFileName);
}

size_byte_t GpFileUtils::SSize (std::string_view aFileName)
{
    return size_byte_t::SMake(NumOps::SConvert<u_int_64>(std::filesystem::file_size(aFileName)));
}

bool    GpFileUtils::SIsPathReadable (std::string_view aPath)
{
    return GpFileImpl::SIsPathReadable(aPath);
}

bool    GpFileUtils::SIsPathWritable (std::string_view aPath)
{
    return GpFileImpl::SIsPathWritable(aPath);
}

}// namespace GPlatform

#endif// GP_USE_FILE_UTILS
