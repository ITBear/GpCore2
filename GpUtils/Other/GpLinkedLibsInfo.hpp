#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <vector>

namespace GPlatform {

struct GpLinkedLibInfo
{
    std::string     iName;
    const size_t    iVersionMaj = {};
    const size_t    iVersionMin = {};
    const size_t    iVersionPat = {};
};

class GP_UTILS_API GpLinkedLibsInfo
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpLinkedLibsInfo)

    using InfoAsTextT = std::vector<std::tuple<std::string/*Name*/, std::string/*Value*/>>;

private:
                                        GpLinkedLibsInfo    (void) noexcept;
                                        ~GpLinkedLibsInfo   (void) noexcept;

public:
    static GpLinkedLibsInfo&            S                   (void) noexcept;

    void                                Register            (std::string    aName,
                                                             size_t         aVersionMaj,
                                                             size_t         aVersionMin,
                                                             size_t         aVersionPat);

    const std::vector<GpLinkedLibInfo>& Libs                (void) const noexcept REQUIRES(iMutex) {return iLibs;}
    InfoAsTextT                         InfoAsText          (void) const;

private:
    mutable GpMutex                     iMutex;
    std::vector<GpLinkedLibInfo>        iLibs GUARDED_BY(iMutex);
};

#if defined(STATIC_LINK)
#   define GP_DECLARE_LIB_REGISTRATOR(LIB_NAME) extern "C" void GP_##LIB_NAME##_REGISTER_LIB_TO_LINKED_LIBS_INFO (void);
#   define GP_CALL_LIB_REGISTRATOR(LIB_NAME)    GP_##LIB_NAME##_REGISTER_LIB_TO_LINKED_LIBS_INFO();
#elif defined(SHARED_LINK)
#   define GP_DECLARE_LIB_REGISTRATOR(LIB_NAME) \
    extern "C" __attribute__((constructor, used)) void GP_##LIB_NAME##_REGISTER_LIB_TO_LINKED_LIBS_INFO (void);
#endif

#define GP_IMPLEMENT_LIB_REGISTRATOR(LIB_NAME) \
 \
void GP_##LIB_NAME##_REGISTER_LIB_TO_LINKED_LIBS_INFO (void) \
{ \
    GpLinkedLibsInfo::S().Register \
    ( \
        std::string(std::string_view(GP_CURRENT_LIB_PACKET_NAME)), \
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_MAJ})), \
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_MIN})), \
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_PAT})) \
    ); \
}

}// namespace GPlatform
