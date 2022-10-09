#pragma once

#include "../GpMacro.hpp"
#include "../SyncPrimitives/GpRWLock.hpp"

#include <map>
#include <any>
#include <optional>
#include <mutex>
#include <shared_mutex>

namespace GPlatform {

class GP_UTILS_API GpGlobals
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGlobals)

public:
    using ContainerT = std::map<std::string, std::any, std::less<>>;


private:
                                    GpGlobals   (void) noexcept;
                                    ~GpGlobals  (void) noexcept;

public:
    static GpGlobals&               S           (void) noexcept {return sGlobals;}

    inline bool                     Set         (std::string    aKey,
                                                 std::any       aValue);
    inline std::optional<std::any>  Get         (std::string_view   aKey) const;
    inline std::optional<std::any>  GetAndClear (std::string_view   aKey);

private:
    mutable GpRWLock        iLockRW;
    ContainerT              iContainer;

    static GpGlobals        sGlobals;
};

bool    GpGlobals::Set
(
    std::string aKey,
    std::any    aValue
)
{
    std::scoped_lock lockRW(iLockRW);

    return iContainer.try_emplace(std::move(aKey), std::move(aValue)).second;
}

std::optional<std::any> GpGlobals::Get (std::string_view aKey) const
{
    std::shared_lock lockW(iLockRW);

    auto iter = iContainer.find(aKey);

    if (iter != iContainer.end())
    {
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

std::optional<std::any> GpGlobals::GetAndClear (std::string_view aKey)
{
    std::scoped_lock lockRW(iLockRW);

    auto iter = iContainer.find(aKey);

    if (iter != iContainer.end())
    {
        std::optional<std::any> res = std::move(iter->second);
        iContainer.erase(iter);
        return res;
    } else
    {
        return std::nullopt;
    }
}

}//GPlatform
