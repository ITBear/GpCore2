#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_FILE_UTILS)

#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpFileUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpFileUtils)

    static GpBytesArray     SReadAll    (std::string_view aFileName);
    static void             SAppend     (std::string_view   aFileName,
                                         GpSpanPtrByteR     aData);
    static void             SCopy       (std::string_view aFrom,
                                         std::string_view aTo);
private:

};

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
