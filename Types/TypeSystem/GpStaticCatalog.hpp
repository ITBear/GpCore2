#pragma once

#include "../Containers/GpElementsCatalog.hpp"
#include "GpTypeStructBase.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

class GPCORE_API GpStaticCatalog
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpStaticCatalog)

    using CatalogT  = GpElementsCatalog<std::string, GpTypeStructBase::SP, GpMap>;

private:
                                GpStaticCatalog     (void) noexcept;
                                ~GpStaticCatalog    (void) noexcept;

public:
    static GpStaticCatalog&     S                   (void);

    void                        Clear               (void) noexcept;

    void                        Register            (std::string_view   aKey, GpTypeStructBase::SP aValue);
    void                        Register            (std::string&&      aKey, GpTypeStructBase::SP aValue);


    GpTypeStructBase::SP        Unregister          (std::string_view aKey);

    GpTypeStructBase::CSP       Find                (std::string_view aKey) const;
    GpTypeStructBase::SP        Find                (std::string_view aKey);

private:
    CatalogT                    iCatalog;
};

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
