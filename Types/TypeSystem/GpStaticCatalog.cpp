#include "GpStaticCatalog.hpp"
#include "../../Exceptions/GpException.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpStaticCatalog::GpStaticCatalog (void) noexcept
{
}

GpStaticCatalog::~GpStaticCatalog (void) noexcept
{
}

GpStaticCatalog&    GpStaticCatalog::S (void)
{
    static GpStaticCatalog sCatalog;

    return sCatalog;
}

void    GpStaticCatalog::Clear (void) noexcept
{
    iCatalog.Clear();
}

void    GpStaticCatalog::Register (std::string_view     aKey,
                                   GpTypeStructBase::SP aValue)
{
    iCatalog.Register(std::string(aKey), std::move(aValue));
}

void    GpStaticCatalog::Register (std::string&&        aKey,
                                   GpTypeStructBase::SP aValue)
{
    iCatalog.Register(std::move(aKey), std::move(aValue));
}

GpTypeStructBase::SP    GpStaticCatalog::Unregister (std::string_view aKey)
{
    return iCatalog.Unregister(aKey);
}

GpTypeStructBase::CSP   GpStaticCatalog::Find (std::string_view aKey) const
{
    return iCatalog.FindOrThrow(aKey);
}

GpTypeStructBase::SP    GpStaticCatalog::Find (std::string_view aKey)
{
    return iCatalog.FindOrThrow(aKey);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
