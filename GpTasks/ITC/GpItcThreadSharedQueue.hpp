#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../GpTasks_global.hpp"
#include "../../GpUtils/Macro/GpMacroTags.hpp"
#include "../../GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"

#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

namespace GPlatform {

template <typename T>
class GpItcThreadSharedQueue
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcThreadSharedQueue)
    TAG_SET(THREAD_SAFE)

    using value_type            = T;
    using this_type             = GpItcThreadSharedQueue<value_type>;
    using underlying_container  = std::queue<value_type>;

public:
                                GpItcThreadSharedQueue  (void) noexcept;
                                ~GpItcThreadSharedQueue (void) noexcept;

    size_t                      Size                    (void) const noexcept;
    bool                        Empty                   (void) const noexcept;
    void                        Clear                   (void);

    void                        PushAndNotifyOne        (const value_type& aValue);
    void                        PushAndNotifyOne        (value_type&& aValue);
    std::optional<value_type>   WaitAndPop              (const milliseconds_t aTimeout);

    //unsafe
    underlying_container&       UnderlyingContainer     (void) noexcept;
    const underlying_container& UnderlyingContainer     (void) const noexcept;

private:
    mutable std::mutex          iLock;
    std::condition_variable     iCV;
    underlying_container        iContainer;
};

template <typename T>

GpItcThreadSharedQueue<T>::GpItcThreadSharedQueue (void) noexcept
{
}

template <typename T>
GpItcThreadSharedQueue<T>::~GpItcThreadSharedQueue (void) noexcept
{
}

template <typename T>
size_t  GpItcThreadSharedQueue<T>::Size (void) const noexcept
{
    std::unique_lock lock(iLock);

    return iContainer.size();
}

template <typename T>
bool    GpItcThreadSharedQueue<T>::Empty (void) const noexcept
{
    std::unique_lock lock(iLock);

    return iContainer.empty();
}

template <typename T>
void    GpItcThreadSharedQueue<T>::Clear (void)
{
    std::unique_lock lock(iLock);

    while (!iContainer.empty())
    {
        iContainer.pop();
    }

    iCV.notify_all();
}

template <typename T>
void    GpItcThreadSharedQueue<T>::PushAndNotifyOne (const value_type& aValue)
{
    std::unique_lock lock(iLock);
    iContainer.push(aValue);
    iCV.notify_one();
}

template <typename T>
void    GpItcThreadSharedQueue<T>::PushAndNotifyOne (value_type&& aValue)
{
    std::unique_lock lock(iLock);
    iContainer.emplace(std::move(aValue));
    iCV.notify_one();
}

template <typename T>
std::optional<typename GpItcThreadSharedQueue<T>::value_type>   GpItcThreadSharedQueue<T>::WaitAndPop (const milliseconds_t aTimeout)
{
    std::unique_lock lock(iLock);
    iCV.wait_for
    (
        lock,
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        [&]{return !iContainer.empty();}
    );

    if (iContainer.empty())
    {
        return std::nullopt;
    }

    value_type e = std::move(iContainer.front());
    iContainer.pop();

    return e;
}

template <typename T>
typename GpItcThreadSharedQueue<T>::underlying_container&   GpItcThreadSharedQueue<T>::UnderlyingContainer (void) noexcept
{
    return iContainer;
}

template <typename T>
const typename GpItcThreadSharedQueue<T>::underlying_container& GpItcThreadSharedQueue<T>::UnderlyingContainer (void) const noexcept
{
    return iContainer;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
