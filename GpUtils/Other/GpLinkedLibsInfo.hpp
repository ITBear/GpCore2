#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <vector>

namespace GPlatform {

struct GpLinkedLibInfo
{
    std::string iName;
    size_t      iVersionMaj = {};
    size_t      iVersionMin = {};
    size_t      iVersionPat = {};
};

class GP_UTILS_API GpLinkedLibsInfo
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpLinkedLibsInfo)

    using InfoAsTextT   = std::vector<std::tuple<std::string/*Name*/, std::string/*Value*/>>;
    using InfoMapT      = boost::container::flat_map<std::string/*Name*/, GpLinkedLibInfo, std::less<>>;

private:
                                GpLinkedLibsInfo    (void) noexcept;
                                ~GpLinkedLibsInfo   (void) noexcept;

public:
    static GpLinkedLibsInfo&    S                   (void) noexcept {return sInstance;}
    static GpLinkedLibsInfo&    _S_                 (void) noexcept;

    void                        Register            (std::string    aName,
                                                     size_t         aVersionMaj,
                                                     size_t         aVersionMin,
                                                     size_t         aVersionPat);

    const InfoMapT&             Libs                (void) const noexcept REQUIRES(iMutex) {return iLibs;}
    InfoAsTextT                 InfoAsText          (void) const;

private:
    mutable GpMutex<>           iMutex;
    InfoMapT                    iLibs GUARDED_BY(iMutex);

    static GpLinkedLibsInfo&    sInstance;
};

}// namespace GPlatform

#define GP_LIB_REGISTRATOR(LIB_NAME) \
    class LIB_NAME \
    { \
    public: \
        static void SRegisterSelf (void) \
        { \
            ::GPlatform::GpLinkedLibsInfo::_S_().Register \
            ( \
                std::string(std::string_view(GP_CURRENT_LIB_PACKET_NAME)), \
                ::GPlatform::NumOps::SConvert<size_t>(::GPlatform::StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_MAJ})), \
                ::GPlatform::NumOps::SConvert<size_t>(::GPlatform::StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_MIN})), \
                ::GPlatform::NumOps::SConvert<size_t>(::GPlatform::StrOps::SToUI64(std::string_view{GP_CURRENT_LIB_VER_PAT})) \
            ); \
        } \
    };
