#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpQueueMPMC.hpp>

#include <queue>
#include <semaphore>

namespace GPlatform {

template <typename T, template<typename, typename...> class Container = std::queue, typename... CArgs>
class GpSharedQueueMPMC final: public GpQueueMPMC<T, Container, CArgs...>
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSharedQueueMPMC)
    using GpSharedQueueT = GpSharedQueueMPMC<T, Container, CArgs...>;
    CLASS_DD(GpSharedQueueT)
    TAG_SET(THREAD_SAFE)

    static constexpr ptrdiff_t SEM_MAX = INT_MAX;

public:
                    GpSharedQueueMPMC   (size_t aMaxSize) noexcept;
    virtual         ~GpSharedQueueMPMC  (void) noexcept override final;

protected:
    virtual void    OnClear             (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                         size_t                         aContainerSize) override final;
    virtual void    OnInterrupt         (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                         size_t                         aContainerSize) override final;
    virtual void    OnStopProduce       (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                         size_t                         aContainerSize) override final;
    virtual void    OnPush              (GpUniqueLock<GpSpinLockRW<>>& aLock) override final;
    virtual void    OnPop               (GpUniqueLock<GpSpinLockRW<>>& aLock) override final;

    virtual bool    WaitForNextPush     (milliseconds_t aTimeout) override final;
    virtual bool    WaitForNextPop      (milliseconds_t aTimeout) override final;

private:
    std::counting_semaphore<SEM_MAX>    iSemaphoreProducers{SEM_MAX};
    std::counting_semaphore<SEM_MAX>    iSemaphoreConsumers{0};
};

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpSharedQueueMPMC<T, Container, CArgs...>::GpSharedQueueMPMC (const size_t aMaxSize) noexcept:
GpQueueMPMC<T, Container>{aMaxSize},
iSemaphoreProducers{ptrdiff_t(aMaxSize)}
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpSharedQueueMPMC<T, Container, CArgs...>::~GpSharedQueueMPMC (void) noexcept
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpSharedQueueMPMC<T, Container, CArgs...>::OnClear
(
    [[maybe_unused]] GpUniqueLock<GpSpinLockRW<>>&  aLock,
    const size_t                                    aContainerSize
)
{
    // Update producers counter
    {
        const size_t freeSize = this->MaxSize() - aContainerSize;
        iSemaphoreProducers.release(ptrdiff_t(freeSize));
    }

    // Update consumers counter
    for (size_t i = 0; i < aContainerSize; ++i)
    {
        iSemaphoreConsumers.try_acquire();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpSharedQueueMPMC<T, Container, CArgs...>::OnInterrupt
(
    [[maybe_unused]] GpUniqueLock<GpSpinLockRW<>>&  aLock,
    const size_t                                    aContainerSize
)
{
    // Update producers counter
    {
        const size_t freeSize = this->MaxSize() - aContainerSize;
        iSemaphoreProducers.release(ptrdiff_t(freeSize));
    }

    // Update consumers counter
    for (size_t i = 0; i < aContainerSize; ++i)
    {
        iSemaphoreConsumers.try_acquire();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpSharedQueueMPMC<T, Container, CArgs...>::OnStopProduce
(
    [[maybe_unused]] GpUniqueLock<GpSpinLockRW<>>&  aLock,
    const size_t                                    aContainerSize
)
{
    // Update producers counter
    {
        const size_t freeSize = this->MaxSize() - aContainerSize;
        iSemaphoreProducers.release(ptrdiff_t(freeSize));
    }

    // Update consumers counter
    for (size_t i = 0; i < aContainerSize; ++i)
    {
        iSemaphoreConsumers.try_acquire();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpSharedQueueMPMC<T, Container, CArgs...>::OnPush ([[maybe_unused]] GpUniqueLock<GpSpinLockRW<>>& aLock)
{
    // Notify one consumer (counter +1)
    iSemaphoreConsumers.release();

    // Update producer (counter -1)
    iSemaphoreProducers.try_acquire();
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpSharedQueueMPMC<T, Container, CArgs...>::OnPop ([[maybe_unused]] GpUniqueLock<GpSpinLockRW<>>& aLock)
{
    // Notify one consumer (counter -1)
    iSemaphoreConsumers.try_acquire();

    // Update producer (counter +1)
    iSemaphoreProducers.release();
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
bool    GpSharedQueueMPMC<T, Container, CArgs...>::WaitForNextPush (const milliseconds_t aTimeout)
{
    return iSemaphoreConsumers.try_acquire_for(std::chrono::milliseconds(aTimeout.As<ssize_t>()));
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
bool    GpSharedQueueMPMC<T, Container, CArgs...>::WaitForNextPop (const milliseconds_t aTimeout)
{
    return iSemaphoreProducers.try_acquire_for(std::chrono::milliseconds(aTimeout.As<ssize_t>()));
}

}// namespace GPlatform
