#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../Units/SI/GpUnitsSI_Time.hpp"
#include "../../Macro/GpMacroTags.hpp"
#include "../../SyncPrimitives/GpConditionVar.hpp"

#include <queue>
#include <optional>

namespace GPlatform {

template <typename T>
class GpSharedQueue
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSharedQueue)
    CLASS_DD(GpSharedQueue<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::queue<value_type>;

public:
                                    GpSharedQueue       (void) noexcept;
                                    GpSharedQueue       (size_t aMaxSize) noexcept;
                                    ~GpSharedQueue      (void) noexcept = default;

    size_t                          MaxSize             (void) const noexcept;
    void                            SetMaxSize          (size_t aMaxSize) noexcept;
    size_t                          Size                (void) const noexcept;
    bool                            Empty               (void) const noexcept;
    void                            Clear               (void);

    [[nodiscard]] bool              Push                (const value_type& aValue);
    [[nodiscard]] bool              Push                (value_type&& aValue);
    [[nodiscard]] bool              PushAndNotifyOne    (const value_type& aValue);
    [[nodiscard]] bool              PushAndNotifyOne    (value_type&& aValue);
    [[nodiscard]] bool              PushAndNotifyAll    (const value_type& aValue);
    [[nodiscard]] bool              PushAndNotifyAll    (value_type&& aValue);

    std::optional<value_type>       Pop                 (void);
    std::optional<value_type>       WaitAndPop          (const milliseconds_t aTimeout);

    underlying_container&           UnderlyingContainer (void) noexcept REQUIRES(iCV);
    const underlying_container&     UnderlyingContainer (void) const noexcept REQUIRES(iCV);

private:
    mutable GpConditionVar          iCV;
    underlying_container            iContainer  GUARDED_BY(iCV.Mutex());
    size_t                          iMaxSize    GUARDED_BY(iCV.Mutex()) = std::numeric_limits<size_t>::max();
};

template <typename T>
GpSharedQueue<T>::GpSharedQueue (void) noexcept
{
}

template <typename T>
GpSharedQueue<T>::GpSharedQueue (size_t aMaxSize) noexcept:
iMaxSize(aMaxSize)
{
}

template <typename T>
size_t  GpSharedQueue<T>::MaxSize (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    return iMaxSize;
}

template <typename T>
void    GpSharedQueue<T>::SetMaxSize (size_t aMaxSize) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    iMaxSize = aMaxSize;
}

template <typename T>
size_t  GpSharedQueue<T>::Size (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    return iContainer.size();
}

template <typename T>
bool    GpSharedQueue<T>::Empty (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    return iContainer.empty();
}

template <typename T>
void    GpSharedQueue<T>::Clear (void)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    while (!iContainer.empty())
    {
        iContainer.pop();
    }

    iCV.NotifyAll();
}

template <typename T>
bool    GpSharedQueue<T>::Push (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

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
bool    GpSharedQueue<T>::Push (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

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
bool    GpSharedQueue<T>::PushAndNotifyOne (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iCV.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpSharedQueue<T>::PushAndNotifyOne (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iCV.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpSharedQueue<T>::PushAndNotifyAll (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iCV.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpSharedQueue<T>::PushAndNotifyAll (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    if (iContainer.size() < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iCV.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
std::optional<typename GpSharedQueue<T>::value_type>    GpSharedQueue<T>::Pop (void)
{
    GpUniqueLock<GpMutex> uniqueLock(iCV.Mutex());

    if (iContainer.empty()) [[unlikely]]
    {
        return std::nullopt;
    }

    value_type e = std::move(iContainer.front());
    iContainer.pop();
    return e;
}

template <typename T>
std::optional<typename GpSharedQueue<T>::value_type>    GpSharedQueue<T>::WaitAndPop (const milliseconds_t aTimeout)
{
    return iCV.WaitFor<value_type>
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS // Check condition
        {
            return !iContainer.empty();
        },
        [&]() NO_THREAD_SAFETY_ANALYSIS // condition met
        {
            value_type e = std::move(iContainer.front());
            iContainer.pop();
            return e;
        },
        aTimeout
    );
}

template <typename T>
typename GpSharedQueue<T>::underlying_container&    GpSharedQueue<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpSharedQueue<T>::underlying_container&  GpSharedQueue<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
