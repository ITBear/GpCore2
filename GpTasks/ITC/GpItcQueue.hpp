#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>

#include <queue>
#include <optional>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template <typename T>
class GpItcQueue
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcQueue)
    CLASS_DD(GpItcQueue<T>)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using underlying_container  = std::queue<value_type>;

public:
                                GpItcQueue          (void) noexcept;
                                GpItcQueue          (size_t aMaxSize) noexcept;
                                ~GpItcQueue         (void) noexcept = default;

    size_t                      MaxSize             (void) const noexcept;
    void                        SetMaxSize          (size_t aMaxSize) noexcept;
    size_t                      Size                (void) const noexcept;
    bool                        Empty               (void) const noexcept;
    void                        Clear               (void);

    void                        Interrupt           (void);

    [[nodiscard]] bool          PushAndNotifyOne    (const value_type& aValue);
    [[nodiscard]] bool          PushAndNotifyOne    (value_type&& aValue);
    [[nodiscard]] bool          PushAndNotifyAll    (const value_type& aValue);
    [[nodiscard]] bool          PushAndNotifyAll    (value_type&& aValue);
    std::optional<value_type>   WaitAndPop          (milliseconds_t aTimeout);

    underlying_container&       UnderlyingContainer (void) noexcept REQUIRES(iItcCondition);
    const underlying_container& UnderlyingContainer (void) const noexcept REQUIRES(iItcCondition);

private:
    mutable GpItcCondition  iItcCondition;
    underlying_container    iContainer      GUARDED_BY(iItcCondition.Mutex());
    size_t                  iMaxSize        GUARDED_BY(iItcCondition.Mutex()) = std::numeric_limits<size_t>::max();
    bool                    iIsInterrupt    GUARDED_BY(iItcCondition.Mutex()) = false;
};

template <typename T>
GpItcQueue<T>::GpItcQueue (void) noexcept
{
}

template <typename T>
GpItcQueue<T>::GpItcQueue (const size_t aMaxSize) noexcept:
iMaxSize{aMaxSize}
{
}

template <typename T>
size_t  GpItcQueue<T>::MaxSize (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return iMaxSize;
}

template <typename T>
void    GpItcQueue<T>::SetMaxSize (const size_t aMaxSize) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    iMaxSize = aMaxSize;
}

template <typename T>
size_t  GpItcQueue<T>::Size (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return std::size(iContainer);
}

template <typename T>
bool    GpItcQueue<T>::Empty (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return iContainer.empty();
}

template <typename T>
void    GpItcQueue<T>::Clear (void)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    while (!iContainer.empty())
    {
        iContainer.pop();
    }

    iItcCondition.NotifyAll();
}

template <typename T>
void    GpItcQueue<T>::Interrupt (void)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};
    iIsInterrupt = true;
    iItcCondition.NotifyAll();
}

template <typename T>
bool    GpItcQueue<T>::PushAndNotifyOne (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iItcCondition.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcQueue<T>::PushAndNotifyOne (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iItcCondition.NotifyOne();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcQueue<T>::PushAndNotifyAll (const value_type& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.push(aValue);
        iItcCondition.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
bool    GpItcQueue<T>::PushAndNotifyAll (value_type&& aValue)
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::move(aValue));
        iItcCondition.NotifyAll();

        return true;
    } else
    {
        return false;
    }
}

template <typename T>
auto    GpItcQueue<T>::WaitAndPop (const milliseconds_t aTimeout) -> std::optional<value_type>
{
    const GpItcCondition::CheckFnT checkFn = [&](void) NO_THREAD_SAFETY_ANALYSIS -> bool // Check condition
    {
        return !iContainer.empty() || iIsInterrupt;
    };

    const GpItcCondition::AtBeginFnT atBeginFn = [](void)
    {
        // NOP
    };

    std::optional<value_type> result;

    const GpItcCondition::AtEndFnT atEndFn = [&](bool aResult) NO_THREAD_SAFETY_ANALYSIS
    {
        if (aResult && !iIsInterrupt) [[likely]]
        {
            result = std::move(iContainer.front());
            iContainer.pop();
        }
    };

    iItcCondition.WaitFor
    (
        checkFn,
        aTimeout,
        atBeginFn,
        atEndFn
    );

    return result;
}

template <typename T>
typename GpItcQueue<T>::underlying_container&   GpItcQueue<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpItcQueue<T>::underlying_container& GpItcQueue<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
