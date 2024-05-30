#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../Macro/GpMacroWarnings.hpp"

#if defined(GP_USE_FILE_UTILS)

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

#include "../Types/Enums/GpEnum.hpp"
#include "../Types/Units/Other/size_byte_t.hpp"
#include "../Types/Strings/GpUTF.hpp"
#include "../Types/Strings/GpStringOps.hpp"
#include "../Types/Containers/GpBytesArray.hpp"

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
    using HandlerT  = HANDLE;
#endif

public:
                        GpFile          (void) noexcept = default;
                        ~GpFile         (void) noexcept {Close();}

    HandlerT            Handler         (void) noexcept {return iHandler;}

    void                Open            (std::string_view   aName,
                                         const GpFileFlags  aFlags);
    bool                IsOpen          (void) const noexcept {return iHandler != HandlerT();}
    void                Close           (void) noexcept;
    void                Flush           (void);
    size_byte_t         Size            (void) const;

    void                GoToPos         (const size_byte_t aPos);
    void                GoToStartPos    (void);
    size_byte_t         GoToEndPos      (void);
    size_byte_t         CurrentPos      (void) const;

    void                Write           (GpSpanByteR    aData);
    void                Read            (GpSpanByteRW   aData);

private:
    HandlerT            iHandler = HandlerT{};
};

}// namespace GPlatform

#endif// GP_USE_FILE_UTILS
