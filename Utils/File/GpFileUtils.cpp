#include "GpFileUtils.hpp"
#include "../../Exceptions/GpException.hpp"
#include "../../Types/Strings/GpStringOps.hpp"

#if defined(GP_USE_FILE_UTILS)

#include <fstream>
#include <filesystem>

namespace GPlatform {

GpBytesArray    GpFileUtils::SReadAll (const GpRawPtrCharR aFileName)
{
    std::string     fileName(aFileName.AsStringView());
    std::ifstream   ifs;

    ifs.open(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    if (   (!ifs.is_open())
        || (ifs.fail())
        || (ifs.bad()))
    {
        THROW_GPE("File '"_sv + fileName + "' not found"_sv);
    }

    const std::ifstream::pos_type   fileSize = ifs.tellg();

    ifs.seekg(0, std::ios::beg);

    GpBytesArray data;
    data.resize(fileSize);
    ifs.read(reinterpret_cast<char*>(data.data()), fileSize);

    return data;
}

void    GpFileUtils::SAppend (const GpRawPtrCharR aFileName,
                              const GpRawPtrByteR aData)
{
    std::string     fileName(aFileName.AsStringView());
    std::ofstream   ofs;

    ofs.open(fileName.c_str(), std::ios::out | std::ios::app | std::ios::binary);

    if (   (!ofs.is_open())
        || (ofs.fail())
        || (ofs.bad()))
    {
        THROW_GPE("File '"_sv + fileName + "' not found"_sv);
    }

    ofs.write(aData.PtrAs<const char*>(), aData.SizeLeft().As<std::streamsize>());
    ofs.flush();
    ofs.close();
}

void    GpFileUtils::SCopy (const GpRawPtrCharR aFrom,
                            const GpRawPtrCharR aTo)
{
    std::filesystem::copy(aFrom.AsStringView(), aTo.AsStringView());
}

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
