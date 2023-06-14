#pragma once

#include "GpDictionary.hpp"
#include "GpAny.hpp"

namespace GPlatform {

class GP_UTILS_API GpGlobalStructCatalogC
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobalStructCatalogC)

    using CatalogT  = GpDictionary<std::u8string, GpAny>;

private:
                                    GpGlobalStructCatalogC  (void) noexcept;
                                    ~GpGlobalStructCatalogC (void) noexcept;

public:
    static GpGlobalStructCatalogC&  S                       (void);

    void                            Clear                   (void) noexcept;

    void                            Register                (std::u8string  aKey,
                                                             GpAny          aValue);

    GpAny                           Unregister              (std::u8string_view aKey);
    const GpAny&                    Find                    (std::u8string_view aKey) const;

    template<typename T>
    T                               FindAs                  (std::u8string_view aKey) const;

private:
    CatalogT                        sInstance;
};

template<typename T>
T   GpGlobalStructCatalogC::FindAs (std::u8string_view aKey) const
{
    const GpAny& val = Find(aKey);
    return val.Value<T>();
}

}//GPlatform
