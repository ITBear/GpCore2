#pragma once

#include "GpElementsCatalog.hpp"

#include <any>

namespace GPlatform {

class GP_UTILS_API GpGlobalStructCatalogC
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobalStructCatalogC)

    using CatalogT  = GpElementsCatalog<std::string, std::any>;

private:
                                    GpGlobalStructCatalogC  (void) noexcept;
                                    ~GpGlobalStructCatalogC (void) noexcept;

public:
    static GpGlobalStructCatalogC&  S                       (void);

    void                            Clear                   (void) noexcept;

    void                            Register                (std::string    aKey,
                                                             std::any       aValue);

    std::any                        Unregister              (std::string_view aKey);
    const std::any&                 Find                    (std::string_view aKey) const;

    template<typename T>
    T                               FindAs                  (std::string_view aKey) const;

private:
    CatalogT                        iCatalog;
};

template<typename T>
T   GpGlobalStructCatalogC::FindAs (std::string_view aKey) const
{
    const std::any& val = Find(aKey);
    return std::any_cast<T>(val);
}

}//GPlatform
