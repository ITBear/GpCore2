#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpQueueMPMC.hpp>

#include <queue>

namespace GPlatform {

template <typename T, template<typename, typename...> class Container = std::queue, typename... CArgs>
class GpItcQueueMPMC final: public GpQueueMPMC<T, Container, CArgs...>
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpItcQueueMPMC)
    using GpItcQueueT = GpItcQueueMPMC<T, Container, CArgs...>;
    CLASS_DD(GpItcQueueT)
    TAG_SET(THREAD_SAFE)

public:
                    GpItcQueueMPMC  (size_t aMaxSize) noexcept;
    virtual         ~GpItcQueueMPMC (void) noexcept override final;

protected:
    virtual void    OnClear         (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS override final;
    virtual void    OnInterrupt     (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS override final;
    virtual void    OnStopProduce   (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS override final;
    virtual void    OnPush          (GpUniqueLock<GpSpinLockRW<>>& aLock) NO_THREAD_SAFETY_ANALYSIS override final;
    virtual void    OnPop           (GpUniqueLock<GpSpinLockRW<>>& aLock) NO_THREAD_SAFETY_ANALYSIS override final;

    virtual bool    WaitForNextPush (milliseconds_t aTimeout) override final;
    virtual bool    WaitForNextPop  (milliseconds_t aTimeout) override final;

private:
    mutable GpItcConditionVar   iItcCvProducers;
    mutable GpItcConditionVar   iItcCvConsumers;
};

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpItcQueueMPMC<T, Container, CArgs...>::GpItcQueueMPMC (const size_t aMaxSize) noexcept:
GpQueueMPMC<T, Container>{aMaxSize}
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpItcQueueMPMC<T, Container, CArgs...>::~GpItcQueueMPMC (void) noexcept
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpItcQueueMPMC<T, Container, CArgs...>::OnClear
(
    GpUniqueLock<GpSpinLockRW<>>&       aLock,
    [[maybe_unused]] const size_t   aContainerSize
)
{
    aLock.unlock();

    {
        GpUniqueLock uniqueLock{iItcCvProducers.SpinLockRW()};
        iItcCvProducers.NotifyAll();
    }

    {
        GpUniqueLock uniqueLock{iItcCvConsumers.SpinLockRW()};
        iItcCvConsumers.NotifyAll();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpItcQueueMPMC<T, Container, CArgs...>::OnInterrupt
(
    GpUniqueLock<GpSpinLockRW<>>&   aLock,
    [[maybe_unused]] const size_t   aContainerSize
)
{
    aLock.unlock();

    {
        GpUniqueLock uniqueLock{iItcCvProducers.SpinLockRW()};
        iItcCvProducers.NotifyAll();
    }

    {
        GpUniqueLock uniqueLock{iItcCvConsumers.SpinLockRW()};
        iItcCvConsumers.NotifyAll();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpItcQueueMPMC<T, Container, CArgs...>::OnStopProduce
(
    GpUniqueLock<GpSpinLockRW<>>&   aLock,
    [[maybe_unused]] const size_t   aContainerSize
)
{
    aLock.unlock();

    {
        GpUniqueLock uniqueLock{iItcCvProducers.SpinLockRW()};
        iItcCvProducers.NotifyAll();
    }

    {
        GpUniqueLock uniqueLock{iItcCvConsumers.SpinLockRW()};
        iItcCvConsumers.NotifyAll();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpItcQueueMPMC<T, Container, CArgs...>::OnPush (GpUniqueLock<GpSpinLockRW<>>& aLock)
{
    aLock.unlock();

    {
        GpUniqueLock uniqueLockConsumers{iItcCvConsumers.SpinLockRW()};
        iItcCvConsumers.NotifyOne();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpItcQueueMPMC<T, Container, CArgs...>::OnPop (GpUniqueLock<GpSpinLockRW<>>& aLock)
{
    aLock.unlock();

    {
        GpUniqueLock uniqueLockConsumers{iItcCvProducers.SpinLockRW()};
        iItcCvProducers.NotifyOne();
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
bool    GpItcQueueMPMC<T, Container, CArgs...>::WaitForNextPush (milliseconds_t aTimeout)
{
    const GpItcConditionVar::CheckFnT checkFn = [&](void) -> bool // Check condition
    {
        GpSharedLock sharedLock{this->SpinLockRW()};

        return (!static_cast<const GpItcQueueMPMC*>(this)->UnderlyingContainer().empty())
            || (this->FlagsNoLock() > 0);
    };

    return iItcCvConsumers.WaitFor
    (
        checkFn,
        aTimeout,
        nullptr,
        nullptr
    );
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
bool    GpItcQueueMPMC<T, Container, CArgs...>::WaitForNextPop (const milliseconds_t aTimeout)
{
    const GpItcConditionVar::CheckFnT checkFn = [&](void) -> bool // Check condition
    {
        GpSharedLock sharedLock{this->SpinLockRW()};

        return (std::size(static_cast<const GpItcQueueMPMC*>(this)->UnderlyingContainer()) < this->MaxSize())
            || (this->FlagsNoLock() > 0);
    };

    return iItcCvProducers.WaitFor
    (
        checkFn,
        aTimeout,
        nullptr,
        nullptr
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
