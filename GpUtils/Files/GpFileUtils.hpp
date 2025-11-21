#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_FILE_UTILS)

#include <GpCore2/GpUtils/Types/Containers/GpByteArray.hpp>
#include <GpCore2/GpUtils/Types/Units/Other/size_byte_t.hpp>

namespace GPlatform {

class GP_UTILS_API GpFileUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpFileUtils)

    static GpByteArray  SReadAll        (std::string_view aFileName);
    static void         SWriteAll       (std::string_view   aFileName,
                                         GpSpanByteR        aData);
    static void         SAppend         (std::string_view   aFileName,
                                         GpSpanByteR        aData);
    static void         SCopy           (std::string_view aFrom,
                                         std::string_view aTo);
    static bool         SIsExists       (std::string_view aFileName);
    static size_byte_t  SSize           (std::string_view aFileName);
    static bool         SIsPathReadable (std::string_view aPath);
    static bool         SIsPathWritable (std::string_view aPath);
};

}// namespace GPlatform

#endif// GP_USE_FILE_UTILS
