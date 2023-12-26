#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpContainersT.hpp"
#include "../../Macro/GpMacroTags.hpp"
#include "../../SyncPrimitives/GpRWSpinLock.hpp"
#include "../../SyncPrimitives/GpMutex.hpp"
#include "../../SyncPrimitives/GpSharedMutex.hpp"

#include <queue>
#include <optional>

namespace GPlatform {

template <typename T>
class GpSharedQueueFast
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSharedQueueFast)
    CLASS_DD(GpSharedQueueFast<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::queue<value_type>;

public:
                                    GpSharedQueueFast   (void) noexcept;
                                    GpSharedQueueFast   (size_t aMaxSize) noexcept;
                                    ~GpSharedQueueFast  (void) noexcept = default;

    size_t                          MaxSize             (void) const noexcept;
    void                            SetMaxSize          (size_t aMaxSize) noexcept;
    size_t                          Size                (void) const noexcept;
    bool                            Empty               (void) const noexcept;
    void                            Clear               (void);

    [[nodiscard]] bool              Push                (const value_type& aValue);
    [[nodiscard]] bool              Push                (value_type&& aValue);

    std::optional<value_type>       Front               (void);
    std::optional<value_type>       Pop                 (void);

    underlying_container&           UnderlyingContainer (void) noexcept REQUIRES(iRwSpinLock);
    const underlying_container&     UnderlyingContainer (void) const noexcept REQUIRES(iRwSpinLock);

private:
    mutable GpRWSpinLock            iRwSpinLock;
    underlying_container            iContainer  GUARDED_BY(iRwSpinLock);
    size_t                          iMaxSize    GUARDED_BY(iRwSpinLock) = std::numeric_limits<size_t>::max();
};

template <typename T>
GpSharedQueueFast<T>::GpSharedQueueFast (void) noexcept
{
}

template <typename T>
GpSharedQueueFast<T>::GpSharedQueueFast (size_t aMaxSize) noexcept:
    iMaxSize(aMaxSize)
{
}

template <typename T>
size_t  GpSharedQueueFast<T>::MaxSize (void) const noexcept
{
    GpSharedLock<GpRWSpinLock> sharedLock(iRwSpinLock);

    return iMaxSize;
}

template <typename T>
void    GpSharedQueueFast<T>::SetMaxSize (size_t aMaxSize) noexcept
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    iMaxSize = aMaxSize;
}

template <typename T>
size_t  GpSharedQueueFast<T>::Size (void) const noexcept
{
    GpSharedLock<GpRWSpinLock> sharedLock(iRwSpinLock);

    return iContainer.size();
}

template <typename T>
bool    GpSharedQueueFast<T>::Empty (void) const noexcept
{
    GpSharedLock<GpRWSpinLock> sharedLock(iRwSpinLock);

    return iContainer.empty();
}

template <typename T>
void    GpSharedQueueFast<T>::Clear (void)
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    while (!iContainer.empty())
    {
        iContainer.pop();
    }
}

template <typename T>
bool    GpSharedQueueFast<T>::Push (const value_type& aValue)
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpSharedQueueFast<T>::Push (value_type&& aValue)
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        return true;
    } else
    {
        return false;
    }
}

template <typename T>
std::optional<typename GpSharedQueueFast<T>::value_type>    GpSharedQueueFast<T>::Front (void)
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    if (iContainer.empty()) [[unlikely]]
    {
        return std::nullopt;
    }

    return iContainer.front();
}

template <typename T>
std::optional<typename GpSharedQueueFast<T>::value_type>    GpSharedQueueFast<T>::Pop (void)
{
    GpUniqueLock<GpRWSpinLock> uniqueLock(iRwSpinLock);

    if (iContainer.empty()) [[unlikely]]
    {
        return std::nullopt;
    }

    value_type e = std::move(iContainer.front());
    iContainer.pop();
    return e;
}

template <typename T>
typename GpSharedQueueFast<T>::underlying_container&    GpSharedQueueFast<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpSharedQueueFast<T>::underlying_container&  GpSharedQueueFast<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
