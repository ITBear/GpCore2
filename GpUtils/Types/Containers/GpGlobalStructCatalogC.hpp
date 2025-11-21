#pragma once

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>

namespace GPlatform {

class GP_UTILS_API GpGlobalStructCatalogC
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobalStructCatalogC)
    TAG_SET(THREAD_SAFE)

    using CatalogT = GpSharedMap<boost::container::small_flat_map<std::string, GpCSP<GpAny>, 32, std::less<>>>;

private:
                                    GpGlobalStructCatalogC  (void) noexcept;
                                    ~GpGlobalStructCatalogC (void) noexcept;

public:
    static GpGlobalStructCatalogC&  S                       (void);


    void                            Clear                   (void) noexcept;
    void                            Register                (std::string    aKey,
                                                             GpAny&&        aValue);
    void                            Unregister              (std::string_view aKey);
    GpCSP<GpAny>                    Find                    (std::string_view aKey) const;

private:
    CatalogT iCatalog;
};

}// namespace GPlatform
