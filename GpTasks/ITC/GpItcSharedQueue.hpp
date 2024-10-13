#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedCondition.hpp>

#include <queue>
#include <optional>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template <typename T>
class GpItcSharedQueue
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcSharedQueue)
    CLASS_DD(GpItcSharedQueue<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::queue<value_type>;

public:
                                    GpItcSharedQueue    (void) noexcept;
                                    GpItcSharedQueue    (size_t aMaxSize) noexcept;
                                    ~GpItcSharedQueue   (void) noexcept = default;

    size_t                          MaxSize             (void) const noexcept;
    void                            SetMaxSize          (size_t aMaxSize) noexcept;
    size_t                          Size                (void) const noexcept;
    bool                            Empty               (void) const noexcept;
    void                            Clear               (void);

    void                            Interrupt           (void);

    [[nodiscard]] bool              PushAndNotifyOne    (const value_type& aValue);
    [[nodiscard]] bool              PushAndNotifyOne    (value_type&& aValue);
    [[nodiscard]] bool              PushAndNotifyAll    (const value_type& aValue);
    [[nodiscard]] bool              PushAndNotifyAll    (value_type&& aValue);
    std::optional<value_type>       WaitAndPop          (milliseconds_t aTimeout);

    underlying_container&           UnderlyingContainer (void) noexcept REQUIRES(iSC);
    const underlying_container&     UnderlyingContainer (void) const noexcept REQUIRES(iSC);

private:
    mutable GpItcSharedCondition    iSC;
    underlying_container            iContainer      GUARDED_BY(iSC.Mutex());
    size_t                          iMaxSize        GUARDED_BY(iSC.Mutex()) = std::numeric_limits<size_t>::max();
    bool                            iIsInterrupt    GUARDED_BY(iSC.Mutex()) = false;
};

template <typename T>
GpItcSharedQueue<T>::GpItcSharedQueue (void) noexcept
{
}

template <typename T>
GpItcSharedQueue<T>::GpItcSharedQueue (size_t aMaxSize) noexcept:
iMaxSize(aMaxSize)
{
}

template <typename T>
size_t  GpItcSharedQueue<T>::MaxSize (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    return iMaxSize;
}

template <typename T>
void    GpItcSharedQueue<T>::SetMaxSize (size_t aMaxSize) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    iMaxSize = aMaxSize;
}

template <typename T>
size_t  GpItcSharedQueue<T>::Size (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    return std::size(iContainer);
}

template <typename T>
bool    GpItcSharedQueue<T>::Empty (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    return iContainer.empty();
}

template <typename T>
void    GpItcSharedQueue<T>::Clear (void)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    while (!iContainer.empty())
    {
        iContainer.pop();
    }

    iSC.NotifyAll();
}

template <typename T>
void    GpItcSharedQueue<T>::Interrupt (void)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};
    iIsInterrupt = true;
    iSC.NotifyAll();
}

template <typename T>
bool    GpItcSharedQueue<T>::PushAndNotifyOne (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iSC.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcSharedQueue<T>::PushAndNotifyOne (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iSC.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcSharedQueue<T>::PushAndNotifyAll (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iSC.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcSharedQueue<T>::PushAndNotifyAll (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iSC.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
std::optional<typename GpItcSharedQueue<T>::value_type> GpItcSharedQueue<T>::WaitAndPop (const milliseconds_t aTimeout)
{
    return iSC.WaitFor<value_type>
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS -> bool // Check condition
        {
            return !iContainer.empty() || iIsInterrupt;
        },
        [&]() NO_THREAD_SAFETY_ANALYSIS -> std::optional<value_type> // condition met
        {
            if (!iIsInterrupt) [[likely]]
            {
                value_type e = std::move(iContainer.front());
                iContainer.pop();
                return e;
            } else
            {
                return std::nullopt;
            }
        },
        aTimeout
    );
}

template <typename T>
typename GpItcSharedQueue<T>::underlying_container& GpItcSharedQueue<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpItcSharedQueue<T>::underlying_container&   GpItcSharedQueue<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
