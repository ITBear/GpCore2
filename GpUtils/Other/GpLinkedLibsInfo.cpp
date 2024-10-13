#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

namespace GPlatform {

GpLinkedLibsInfo::GpLinkedLibsInfo (void) noexcept
{
}

GpLinkedLibsInfo::~GpLinkedLibsInfo (void) noexcept
{
}

GpLinkedLibsInfo&   GpLinkedLibsInfo::S (void) noexcept
{
    static GpLinkedLibsInfo sInstance;

    return sInstance;
}

void    GpLinkedLibsInfo::Register
(
    std::string     aName,
    const size_t    aVersionMaj,
    const size_t    aVersionMin,
    const size_t    aVersionPat
)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    iLibs.emplace_back
    (
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
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    InfoAsTextT infoAsText;
    infoAsText.reserve(std::size(iLibs));

    for (const GpLinkedLibInfo& info: iLibs)
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
