#include "GpGlobalStructCatalog.hpp"
#include "../../Exceptions/GpException.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpGlobalStructCatalog::GpGlobalStructCatalog (void) noexcept
{
}

GpGlobalStructCatalog::~GpGlobalStructCatalog (void) noexcept
{
}

GpGlobalStructCatalog&  GpGlobalStructCatalog::S (void)
{
    static GpGlobalStructCatalog sCatalog;

    return sCatalog;
}

void    GpGlobalStructCatalog::Clear (void) noexcept
{
    iCatalog.Clear();
}

void    GpGlobalStructCatalog::Register
(
    std::string_view        aKey,
    GpTypeStructBase::SP    aValue
)
{
    iCatalog.Register(aKey, std::move(aValue));
}

void    GpGlobalStructCatalog::Register
(
    std::string&&           aKey,
    GpTypeStructBase::SP    aValue
)
{
    iCatalog.Register(std::move(aKey), std::move(aValue));
}

GpTypeStructBase::SP    GpGlobalStructCatalog::Unregister (std::string_view aKey)
{
    return iCatalog.Unregister(aKey);
}

GpTypeStructBase::CSP   GpGlobalStructCatalog::Find (std::string_view aKey) const
{
    return iCatalog.Find(aKey);
}

GpTypeStructBase::SP    GpGlobalStructCatalog::Find (std::string_view aKey)
{
    return iCatalog.Find(aKey);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
