#include "GpGlobalStructCatalogC.hpp"

namespace GPlatform {

GpGlobalStructCatalogC::GpGlobalStructCatalogC (void) noexcept
{
}

GpGlobalStructCatalogC::~GpGlobalStructCatalogC (void) noexcept
{
}

GpGlobalStructCatalogC& GpGlobalStructCatalogC::S (void)
{
    static GpGlobalStructCatalogC sInstance;

    return sInstance;
}

void    GpGlobalStructCatalogC::Clear (void) noexcept
{
    sInstance.Clear();
}

void    GpGlobalStructCatalogC::Register
(
    std::string aKey,
    GpAny       aValue
)
{
    sInstance.SetOrUpdate
    (
        std::move(aKey),
        std::move(aValue)
    );
}

GpAny   GpGlobalStructCatalogC::Unregister (std::string_view aKey)
{
    return sInstance.Erase(aKey);
}

const GpAny GpGlobalStructCatalogC::Find (std::string_view aKey) const
{
    return sInstance.Get(aKey);
}

std::optional<const GpAny>  GpGlobalStructCatalogC::FindOpt (std::string_view aKey) const
{
    return sInstance.GetOpt(aKey);
}

}// namespace GPlatform
