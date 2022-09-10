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
    static GpGlobalStructCatalogC sCatalog;

    return sCatalog;
}

void    GpGlobalStructCatalogC::Clear (void) noexcept
{
    iCatalog.Clear();
}

void    GpGlobalStructCatalogC::Register
(
    std::string aKey,
    std::any    aValue
)
{
    iCatalog.Register(std::move(aKey), std::move(aValue));
}

std::any    GpGlobalStructCatalogC::Unregister (std::string_view aKey)
{
    return iCatalog.Unregister(aKey);
}

const std::any& GpGlobalStructCatalogC::Find (std::string_view aKey) const
{
    return iCatalog.Find(aKey);
}

}//GPlatform
