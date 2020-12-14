#pragma once

#include "../Containers/GpElementsCatalog.hpp"
#include "GpTypeStructBase.hpp"
#include "GpTypeManager.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

class GPCORE_API GpStaticCatalogC
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpStaticCatalogC)

    using CatalogT  = GpElementsCatalog<std::string, GpTypeStructBase::CSP, GpMap>;

private:
                                GpStaticCatalogC    (void) noexcept;
                                ~GpStaticCatalogC   (void) noexcept;

public:
    static GpStaticCatalogC&    S                   (void);

    void                        Clear               (void) noexcept;

    void                        Register            (std::string_view   aKey, GpTypeStructBase::CSP aValue);
    void                        Register            (std::string&&      aKey, GpTypeStructBase::CSP aValue);


    GpTypeStructBase::CSP       Unregister          (std::string_view aKey);

    GpTypeStructBase::CSP       Find                (std::string_view aKey) const;

    template<typename T>
    T                           FindAs              (std::string_view aKey) const;

private:
    CatalogT                    iCatalog;
};

template<typename T>
T   GpStaticCatalogC::FindAs (std::string_view aKey) const
{
    GpTypeStructBase::CSP val = Find(aKey);
    return GpTypeManager::S().CastSP<T>(val);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
