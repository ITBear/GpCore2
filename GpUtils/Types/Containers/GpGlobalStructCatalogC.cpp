#include <GpCore2/GpUtils/Types/Containers/GpGlobalStructCatalogC.hpp>

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
    iCatalog.Clear();
}

void    GpGlobalStructCatalogC::Register
(
    std::string aKey,
    GpAny&&     aValue
)
{
    iCatalog.Set
    (
        std::move(aKey),
        GpCSP<GpAny>::SNew(std::move(aValue))
    );
}

void    GpGlobalStructCatalogC::Unregister (std::string_view aKey)
{
    iCatalog.Erase(aKey);
}

GpCSP<GpAny>    GpGlobalStructCatalogC::Find (std::string_view aKey) const
{
    try
    {
        return iCatalog.Find(aKey);
    } catch (...)
    {
        return nullptr;
    }
}

}// namespace GPlatform
