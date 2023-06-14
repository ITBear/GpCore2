#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_FILE_UTILS)

#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpFileUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpFileUtils)

    static GpBytesArray     SReadAll    (std::u8string_view aFileName);
    static void             SWriteAll   (std::u8string_view aFileName,
                                         GpSpanPtrByteR     aData);
    static void             SAppend     (std::u8string_view aFileName,
                                         GpSpanPtrByteR     aData);
    static void             SCopy       (std::u8string_view aFrom,
                                         std::u8string_view aTo);
    static bool             SIsExists   (std::u8string_view aFileName);
private:

};

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
