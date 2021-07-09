#pragma once

#include "../Containers/GpElementsCatalog.hpp"
#include "GpTypeStructBase.hpp"
#include "GpTypeManager.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

class GPCORE_API GpGlobalStructCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobalStructCatalog)

    using CatalogT  = GpElementsCatalog<std::string, GpTypeStructBase::SP>;

private:
                                    GpGlobalStructCatalog   (void) noexcept;
                                    ~GpGlobalStructCatalog  (void) noexcept;

public:
    static GpGlobalStructCatalog&   S                       (void);

    void                            Clear                   (void) noexcept;

    void                            Register                (std::string_view       aKey,
                                                             GpTypeStructBase::SP   aValue);
    void                            Register                (std::string&&          aKey,
                                                             GpTypeStructBase::SP   aValue);

    GpTypeStructBase::SP            Unregister              (std::string_view aKey);

    GpTypeStructBase::CSP           Find                    (std::string_view aKey) const;
    GpTypeStructBase::SP            Find                    (std::string_view aKey);

    template<typename T>
    T                               FindAs                  (std::string_view aKey) const;
    template<typename T>
    T                               FindAs                  (std::string_view aKey);

private:
    CatalogT                        iCatalog;
};

template<typename T>
T   GpGlobalStructCatalog::FindAs (std::string_view aKey) const
{
    GpTypeStructBase::CSP val = Find(aKey);
    return GpTypeManager::S().CastSP<T>(val);
}

template<typename T>
T   GpGlobalStructCatalog::FindAs (std::string_view aKey)
{
    GpTypeStructBase::SP val = Find(aKey);
    return GpTypeManager::S().CastSP<T>(val);
}

}//GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
