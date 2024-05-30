// GTags:
// #DOC_ADDED     (2024-05-24)
// #DOC_VALIDATED (2024-05-24)

#pragma once

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>

namespace GPlatform {

/**
 * @brief Global structure catalog class.
 *
 * This class manages a global catalog of structures identified by string keys.
 */
class GP_UTILS_API GpGlobalStructCatalogC
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobalStructCatalogC)
    TAG_SET(THREAD_SAFE)

    /**
     * @brief Type alias for the catalog dictionary.
     */
    using CatalogT = GpDictionary<boost::container::small_flat_map<std::string, GpAny, 32, std::less<>>>;

private:
    /**
     * @brief Default constructor.
     */
                                    GpGlobalStructCatalogC  (void) noexcept;

    /**
     * @brief Destructor.
     */
                                    ~GpGlobalStructCatalogC (void) noexcept;

public:
    /**
     * @brief Get the singleton instance of the catalog.
     * @return Reference to the singleton instance.
     */
    static GpGlobalStructCatalogC&  S                       (void);

    /**
     * @brief Clear all entries in the catalog.
     */
    void                            Clear                   (void) noexcept;

    /**
     * @brief Register a new entry in the catalog.
     * @param aKey The key associated with the entry.
     * @param aValue The value to store in the catalog.
     */
    void                            Register                (std::string    aKey,
                                                             GpAny          aValue);

    /**
     * @brief Unregister an entry from the catalog.
     * @param aKey The key associated with the entry to remove.
     * @return The value that was removed from the catalog.
     */
    GpAny                           Unregister              (std::string_view aKey);

    /**
     * @brief Find an entry in the catalog. Throws exception if not found.
     * @param aKey The key associated with the entry to find.
     * @return The value associated with the key.
     */
    const GpAny                     Find                    (std::string_view aKey) const;

    /**
     * @brief Find an entry in the catalog if it exists.
     * @param aKey The key associated with the entry to find.
     * @return An optional containing the value if it exists, otherwise std::nullopt.
     */
    std::optional<const GpAny>      FindOpt                 (std::string_view aKey) const;

    /**
     * @brief Find an entry in the catalog and cast it to a specified type. Throws exception if not found.
     * @tparam T The type to cast the entry to.
     * @param aKey The key associated with the entry to find.
     * @return The entry casted to the specified type.
     */
    template<typename T>
    T                               FindAs                  (std::string_view aKey) const;

    /**
     * @brief Find an entry in the catalog and cast it to a specified type if it exists.
     * @tparam T The type to cast the entry to.
     * @param aKey The key associated with the entry to find.
     * @return An optional containing the entry casted to the specified type if it exists, otherwise std::nullopt.
     */
    template<typename T>
    std::optional<T>                FindAsOpt               (std::string_view aKey) const;

private:
    CatalogT                        sInstance;
};

template<typename T>
T   GpGlobalStructCatalogC::FindAs (std::string_view aKey) const
{
    const GpAny& val = Find(aKey);
    return val.Value<T>();
}

template<typename T>
std::optional<T>    GpGlobalStructCatalogC::FindAsOpt (std::string_view aKey) const
{
    const auto valOpt = FindOpt(aKey);

    if (!valOpt.has_value()) [[unlikely]]
    {
        return std::nullopt;
    }

    return valOpt.value().Value<T>();
}

}// namespace GPlatform
