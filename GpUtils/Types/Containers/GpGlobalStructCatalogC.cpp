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
    std::u8string   aKey,
    GpAny       aValue
)
{
    sInstance.Set(std::move(aKey), std::move(aValue));
}

GpAny   GpGlobalStructCatalogC::Unregister (std::u8string_view aKey)
{
    return sInstance.Erase(aKey);
}

const GpAny&    GpGlobalStructCatalogC::Find (std::u8string_view aKey) const
{
    return sInstance.Get(aKey);
}

}//GPlatform
