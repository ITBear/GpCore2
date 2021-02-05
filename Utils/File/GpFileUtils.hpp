#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_FILE_UTILS)

#include "../../GpCore_global.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"
#include "../../Types/Containers/GpBytesArray.hpp"
#include "../../Types/Containers/GpRawPtrByte.hpp"

namespace GPlatform {

class GPCORE_API GpFileUtils
{
public:
    CLASS_REMOVE_CTRS(GpFileUtils)

    static GpBytesArray     SReadAll    (const GpRawPtrCharR aFileName);
    static void             SAppend     (const GpRawPtrCharR aFileName,
                                         const GpRawPtrByteR aData);
    static void             SCopy       (const GpRawPtrCharR aFrom,
                                         const GpRawPtrCharR aTo);
private:

};

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
