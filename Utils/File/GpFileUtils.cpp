#include "GpFileUtils.hpp"

#if defined(GP_USE_FILE_UTILS)

#include <fstream>

namespace GPlatform {

GpBytesArray    GpFileUtils::SReadAll (const GpRawPtrCharR aFileName)
{
    std::string fileName(aFileName.AsStringView());

    std::ifstream                   ifs(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    const std::ifstream::pos_type   fileSize = ifs.tellg();

    ifs.seekg(0, std::ios::beg);

    GpBytesArray data;
    data.resize(fileSize);
    ifs.read(reinterpret_cast<char*>(data.data()), fileSize);

    return data;
}

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
