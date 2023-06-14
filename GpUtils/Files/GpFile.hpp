#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_FILE_UTILS)

#include "../Types/Containers/GpDictionary.hpp"
#include "../Types/Enums/GpEnumFlags.hpp"
#include "../Types/Units/Other/size_byte_t.hpp"
#include "../Types/Strings/GpUTF.hpp"

namespace GPlatform {

GP_ENUM(GP_UTILS_API, GpFileFlag,
    READ,
    WRITE,
    CREATE,
    APPEND,
    TRUNCATE,
    DO_NOT_REWRITE_EXISTED
);

using GpFileFlags = GpEnumFlagsST<GpFileFlag>;

class GP_UTILS_API GpFile
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpFile)
    CLASS_DD(GpFile)

#if defined(GP_POSIX)
    using HandlerT  = int;
#elif defined(GP_OS_WINDOWS)
    using HandlerT  = HFILE;
#endif

public:
                        GpFile          (void) noexcept = default;
                        ~GpFile         (void) noexcept {Close();}

    HandlerT            Handler         (void) noexcept {return iHandler;}

    void                Open            (std::u8string_view aName,
                                         const GpFileFlags  aFlags);
    bool                IsOpen          (void) const noexcept {return iHandler != HandlerT();}
    void                Close           (void) noexcept;
    void                Flush           (void);
    size_byte_t         Size            (void) const;

    void                GoToPos         (const size_byte_t aPos);
    void                GoToStartPos    (void);
    size_byte_t         GoToEndPos      (void);
    size_byte_t         CurrentPos      (void) const;

    void                Write           (GpSpanPtrByteR     aData);
    void                Read            (GpSpanPtrByteRW    aData);

private:
    HandlerT            iHandler = HandlerT();
};

}//namespace GPlatform

#endif//GP_USE_FILE_UTILS
