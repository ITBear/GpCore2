#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

namespace GPlatform {

GpLinkedLibsInfo&   GpLinkedLibsInfo::sInstance = GpLinkedLibsInfo::_S_();

GpLinkedLibsInfo::GpLinkedLibsInfo (void) noexcept
{
}

GpLinkedLibsInfo::~GpLinkedLibsInfo (void) noexcept
{
}

GpLinkedLibsInfo&   GpLinkedLibsInfo::_S_ (void) noexcept
{
    static GpLinkedLibsInfo sLinkedLibsInfo;

    return sLinkedLibsInfo;
}

void    GpLinkedLibsInfo::Register
(
    std::string     aName,
    const size_t    aVersionMaj,
    const size_t    aVersionMin,
    const size_t    aVersionPat
)
{
    GpUniqueLock uniqueLock{iMutex};

    iLibs.try_emplace
    (
        aName,
        GpLinkedLibInfo
        {
            .iName          = aName,
            .iVersionMaj    = aVersionMaj,
            .iVersionMin    = aVersionMin,
            .iVersionPat    = aVersionPat
        }
    );
}

GpLinkedLibsInfo::InfoAsTextT   GpLinkedLibsInfo::InfoAsText (void) const
{
    GpUniqueLock uniqueLock{iMutex};

    InfoAsTextT infoAsText;
    infoAsText.reserve(std::size(iLibs));

    for (const auto&[name, info]: iLibs)
    {
        infoAsText.emplace_back
        (
            info.iName,
            fmt::format("{}.{}.{}", info.iVersionMaj, info.iVersionMin, info.iVersionPat)
        );
    }

    return infoAsText;
}

}// namespace GPlatform
