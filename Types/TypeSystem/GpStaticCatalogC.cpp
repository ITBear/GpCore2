#include "GpStaticCatalogC.hpp"
#include "../../Exceptions/GpException.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpStaticCatalogC::GpStaticCatalogC (void) noexcept
{
}

GpStaticCatalogC::~GpStaticCatalogC (void) noexcept
{
}

GpStaticCatalogC&   GpStaticCatalogC::S (void)
{
    static GpStaticCatalogC sCatalog;

    return sCatalog;
}

void    GpStaticCatalogC::Clear (void) noexcept
{
    iCatalog.Clear();
}

void    GpStaticCatalogC::Register (std::string_view        aKey,
                                    GpTypeStructBase::CSP   aValue)
{
    iCatalog.Register(std::string(aKey), std::move(aValue));
}

void    GpStaticCatalogC::Register (std::string&&           aKey,
                                    GpTypeStructBase::CSP   aValue)
{
    iCatalog.Register(std::move(aKey), std::move(aValue));
}

GpTypeStructBase::CSP   GpStaticCatalogC::Unregister (std::string_view aKey)
{
    std::string key(aKey);

    return iCatalog.Unregister(key);
}

GpTypeStructBase::CSP   GpStaticCatalogC::Find (std::string_view aKey) const
{
    std::string key(aKey);
    return iCatalog.FindOrThrow(key);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
