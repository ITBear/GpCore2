#pragma once

#include <GpCore2/GpUtils/Files/GpFile.hpp>

#if defined(GP_USE_FILE_MEMORY_MAP)

namespace GPlatform {

class GP_UTILS_API GpFileMemMap
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpFileMemMap)
    CLASS_DD(GpFileMemMap)

public:
                    GpFileMemMap    (void) noexcept;
                    ~GpFileMemMap   (void) noexcept;

    GpSpanByteRW    OpenAndMap      (std::string_view   aFileName,
                                     GpFileFlags        aFileFlags,
                                     size_byte_t        aOffsetToMap,
                                     size_byte_t        aSizeToMap);
    GpSpanByteRW    ReopenAndMap    (size_byte_t aOffsetToMap,
                                     size_byte_t aSizeToMap);
    void            Close           (void) noexcept;

private:
    void            MemMap          (size_byte_t aOffsetToMap,
                                     size_byte_t aSizeToMap);
    void            MemUnmap        (void) noexcept;

private:
    GpFile          iFile;
    GpSpanByteRW    iMappedData;

#if defined(GP_OS_WINDOWS)
    GpFile::HandlerT    iMappedHandle   = {};
#endif
};

}// namespace GPlatform

#endif// #if defined(GP_USE_FILE_MEMORY_MAP)
