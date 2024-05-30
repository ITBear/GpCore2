#include "GpFileUtils.hpp"
#include "GpFile.hpp"
#include "../Exceptions/GpException.hpp"
#include "../Types/Strings/GpStringOps.hpp"

#if defined(GP_USE_FILE_UTILS)

#include <fstream>
#include <filesystem>

namespace GPlatform {

GpBytesArray    GpFileUtils::SReadAll (std::string_view aFileName)
{
    GpFile  file;
    file.Open(aFileName, {GpFileFlag::READ});
    const size_byte_t fileSize = file.Size();
    GpBytesArray data;
    data.resize(fileSize.Value());
    file.Read(data);
    file.Close();

    return data;

    //std::string   fileName(aFileName);
    //std::ifstream ifs;

    //ifs.open(std::filesystem::path(fileName.c_str()), std::ios::in | std::ios::binary | std::ios::ate);

    //if (   (!ifs.is_open())
    //  || (ifs.fail())
    //  || (ifs.bad()))
    //{
    //  THROW_GP("File '"_sv + fileName + "' not found"_sv);
    //}

    //const std::ifstream::pos_type fileSize = ifs.tellg();

    //ifs.seekg(0, std::ios::beg);

    //GpBytesArray data;
    //data.resize(NumOps::SConvert<size_t>(fileSize));
    //ifs.read(reinterpret_cast<char*>(std::data(data)), fileSize);

    //return data;
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

    //std::string       fileName(aFileName);
    //std::ofstream ofs;

    //ofs.open(std::filesystem::path(fileName.c_str()), std::ios::out | std::ios::app | std::ios::binary);

    //if (   (!ofs.is_open())
    //  || (ofs.fail())
    //  || (ofs.bad()))
    //{
    //  THROW_GP("File '"_sv + fileName + "' not found"_sv);
    //}

    //ofs.write(aData.PtrAs<const char*>(), aData.Size().As<std::streamsize>());
    //ofs.flush();
    //ofs.close();
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

}// namespace GPlatform

#endif// GP_USE_FILE_UTILS
