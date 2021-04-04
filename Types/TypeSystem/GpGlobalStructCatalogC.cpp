#include "GpGlobalStructCatalogC.hpp"
#include "../../Exceptions/GpException.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

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
    std::string_view        aKey,
    GpTypeStructBase::CSP   aValue
)
{
    iCatalog.Register(aKey, std::move(aValue));
}

void    GpGlobalStructCatalogC::Register
(
    std::string&&           aKey,
    GpTypeStructBase::CSP   aValue
)
{
    iCatalog.Register(std::move(aKey), std::move(aValue));
}

GpTypeStructBase::CSP   GpGlobalStructCatalogC::Unregister (std::string_view aKey)
{
    return iCatalog.Unregister(aKey);
}

GpTypeStructBase::CSP   GpGlobalStructCatalogC::Find (std::string_view aKey) const
{
    return iCatalog.Find(aKey);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
