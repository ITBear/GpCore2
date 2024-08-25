#pragma once

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"
#include "../SyncPrimitives/GpMutex.hpp"
#include "../Types/Strings/GpStringOps.hpp"

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
                                                             const size_t   aVersionMaj,
                                                             const size_t   aVersionMin,
                                                             const size_t   aVersionPat);

    const std::vector<GpLinkedLibInfo>& Libs                (void) const noexcept REQUIRES(iMutex) {return iLibs;}
    InfoAsTextT                         InfoAsText          (void) const;

private:
    mutable GpMutex                     iMutex;
    std::vector<GpLinkedLibInfo>        iLibs GUARDED_BY(iMutex);
};

inline void GP_REGISTER_CURRENT_LIB_TO_LINKED_LIBS_INFO (void)
{
    GpLinkedLibsInfo::S().Register
    (
        std::string(std::string_view(GP_CURRENT_LIB_PACKET_NAME)),
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view(GP_CURRENT_LIB_VER_MAJ))),
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view(GP_CURRENT_LIB_VER_MIN))),
        NumOps::SConvert<size_t>(StrOps::SToUI64(std::string_view(GP_CURRENT_LIB_VER_PAT)))
    );
}

#define GP_DECLARE_LIB(EXPORT_API_NAME, LIB_CLASS_NAME) \
 \
class EXPORT_API_NAME LIB_CLASS_NAME \
{ \
private: \
                            LIB_CLASS_NAME  (void) noexcept; \
                            ~LIB_CLASS_NAME (void) noexcept; \
 \
private: \
    static LIB_CLASS_NAME   sInstance; \
};

#define GP_IMPLEMENT_LIB(LIB_CLASS_NAME) \
 \
LIB_CLASS_NAME LIB_CLASS_NAME::sInstance; \
 \
LIB_CLASS_NAME::LIB_CLASS_NAME (void) noexcept \
{ \
    GP_REGISTER_CURRENT_LIB_TO_LINKED_LIBS_INFO(); \
} \
 \
LIB_CLASS_NAME::~LIB_CLASS_NAME (void) noexcept \
{ \
}

}// GPlatform
