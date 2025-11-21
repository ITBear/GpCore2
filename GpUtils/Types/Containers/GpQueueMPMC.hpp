#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>

namespace GPlatform {

template <typename T, template<typename, typename...> class Container, typename... CArgs>
class GpQueueMPMC
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpQueueMPMC)
    TAG_SET(THREAD_SAFE)

    using value_type        = T;
    using underlying_container  = Container<T, CArgs...>;

    enum class FlagE: size_t
    {
        INTERRUPT       = 1 << 0,
        STOP_PRODUCE    = 1 << 1,
        QUEUE_IS_FULL   = 1 << 2,
    };

    using FlagsT = size_t;

    struct PopResT
    {
        std::optional<value_type>   iValue;
        FlagsT                      iFlags;
    };

protected:
                                GpQueueMPMC         (size_t aMaxSize) noexcept;

public:
    virtual                     ~GpQueueMPMC        (void) noexcept;

    size_t                      MaxSize             (void) const noexcept;
    size_t                      Size                (void) const noexcept;
    bool                        Empty               (void) const noexcept;
    void                        Clear               (void);

    void                        Interrupt           (void);
    void                        StopProduce         (void);
    FlagsT                      Flags               (void) const noexcept;

    template<typename V>
    [[nodiscard]] FlagsT        Push                (V&& aValue);

    template<typename V>
    [[nodiscard]] FlagsT        PushWaitFor         (V&&            aValue,
                                                     milliseconds_t aTimeout);

    PopResT                     Pop                 (void);
    PopResT                     PopWaitFor          (milliseconds_t aTimeout);

protected:
    GpSpinLockRW<>&             SpinLockRW          (void) const noexcept RETURN_CAPABILITY(iSpinLockRW) {return iSpinLockRW;}
    const underlying_container& UnderlyingContainer (void) const noexcept REQUIRES_SHARED(iSpinLockRW) {return iContainer;}
    underlying_container&       UnderlyingContainer (void) noexcept REQUIRES(iSpinLockRW) {return iContainer;}
    FlagsT                      FlagsNoLock         (void) const noexcept REQUIRES_SHARED(iSpinLockRW) {return iFlags;}

    virtual void                OnClear             (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS = 0;
    virtual void                OnInterrupt         (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS = 0;
    virtual void                OnStopProduce       (GpUniqueLock<GpSpinLockRW<>>&  aLock,
                                                     size_t                         aContainerSize) NO_THREAD_SAFETY_ANALYSIS = 0;
    virtual void                OnPush              (GpUniqueLock<GpSpinLockRW<>>& aLock) NO_THREAD_SAFETY_ANALYSIS = 0;
    virtual void                OnPop               (GpUniqueLock<GpSpinLockRW<>>& aLock) NO_THREAD_SAFETY_ANALYSIS = 0;
    virtual bool                WaitForNextPush     (milliseconds_t aTimeout) = 0;
    virtual bool                WaitForNextPop      (milliseconds_t aTimeout) = 0;

private:
    mutable GpSpinLockRW<>  iSpinLockRW;
    underlying_container    iContainer      GUARDED_BY(iSpinLockRW);
    FlagsT                  iFlags          GUARDED_BY(iSpinLockRW) = 0;
    const size_t            iMaxSize = ::std::numeric_limits<size_t>::max();
};

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpQueueMPMC<T, Container, CArgs...>::GpQueueMPMC (const size_t aMaxSize) noexcept:
iMaxSize{aMaxSize}
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
GpQueueMPMC<T, Container, CArgs...>::~GpQueueMPMC (void) noexcept
{
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
size_t  GpQueueMPMC<T, Container, CArgs...>::MaxSize (void) const noexcept
{
    return iMaxSize;
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
size_t  GpQueueMPMC<T, Container, CArgs...>::Size (void) const noexcept
{
    GpSharedLock sharedLock{iSpinLockRW};

    return std::size(iContainer);
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
bool    GpQueueMPMC<T, Container, CArgs...>::Empty (void) const noexcept
{
    GpSharedLock sharedLock{iSpinLockRW};

    return iContainer.empty();
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpQueueMPMC<T, Container, CArgs...>::Clear (void)
{
    [[maybe_unused]] underlying_container tmp;

    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        const size_t containerSize = std::size(iContainer);
        if (containerSize > 0)
        {
            tmp.swap(iContainer);
            OnClear(uniqueLock, containerSize);
        }
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpQueueMPMC<T, Container, CArgs...>::Interrupt (void)
{
    GpUniqueLock uniqueLock{iSpinLockRW};
    iFlags |= FlagsT(FlagE::INTERRUPT) | FlagsT(FlagE::STOP_PRODUCE);

    OnInterrupt(uniqueLock, std::size(iContainer));
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
void    GpQueueMPMC<T, Container, CArgs...>::StopProduce (void)
{
    GpUniqueLock uniqueLock{iSpinLockRW};
    iFlags |= FlagsT(FlagE::STOP_PRODUCE);

    OnStopProduce(uniqueLock, std::size(iContainer));
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
auto    GpQueueMPMC<T, Container, CArgs...>::Flags (void) const noexcept -> FlagsT
{
    GpSharedLock sharedLock{iSpinLockRW};

    return iFlags;
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
template<typename V>
auto    GpQueueMPMC<T, Container, CArgs...>::Push (V&& aValue) -> FlagsT
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    if ((iFlags & (FlagsT(FlagE::INTERRUPT) | FlagsT(FlagE::STOP_PRODUCE))) != 0) [[unlikely]] // INTERRUPT or STOP_PRODUCE
    {
        return iFlags;
    }

    if (std::size(iContainer) < iMaxSize) [[likely]]
    {
        iContainer.emplace(std::forward<V>(aValue));
        const auto flags = iFlags;
        OnPush(uniqueLock);

        return flags;
    } else
    {
        return iFlags | FlagsT(FlagE::QUEUE_IS_FULL);
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
template<typename V>
auto    GpQueueMPMC<T, Container, CArgs...>::PushWaitFor
(
    V&&                     aValue,
    const milliseconds_t    aTimeout
) -> FlagsT
{
    // First try
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        if (iFlags & (FlagsT(FlagE::INTERRUPT) | FlagsT(FlagE::STOP_PRODUCE))) [[unlikely]] // INTERRUPT or STOP_PRODUCE
        {
            return iFlags;
        }

        if (std::size(iContainer) < iMaxSize) [[likely]]
        {
            iContainer.emplace(std::forward<V>(aValue));
            const auto flags = iFlags;
            OnPush(uniqueLock);

            return flags;
        }
    }

    // Queue is full, wait for
    if (WaitForNextPop(aTimeout) == true) [[likely]]
    {
        return Push(std::forward<V>(aValue));
    } else
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        return iFlags | FlagsT(FlagE::QUEUE_IS_FULL);
    }
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
auto    GpQueueMPMC<T, Container, CArgs...>::Pop (void) -> PopResT
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    if (iFlags & FlagsT(FlagE::INTERRUPT)) [[unlikely]] // INTERRUPT
    {
        return PopResT
        {
            .iValue = std::nullopt,
            .iFlags = iFlags
        };
    }

    if (iContainer.empty()) [[unlikely]]
    {
        return PopResT
        {
            .iValue = std::nullopt,
            .iFlags = iFlags
        };
    }

    PopResT res
    {
        .iValue = std::move(iContainer.front()),
        .iFlags = iFlags
    };

    iContainer.pop();
    OnPop(uniqueLock);

    return res;
}

template <typename T, template<typename, typename...> class Container, typename... CArgs>
auto    GpQueueMPMC<T, Container, CArgs...>::PopWaitFor (const milliseconds_t aTimeout) -> PopResT
{
    // First try
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        if (iFlags & FlagsT(FlagE::INTERRUPT)) [[unlikely]] // INTERRUPT
        {
            return PopResT
            {
                .iValue = std::nullopt,
                .iFlags = iFlags
            };
        }

        if (!iContainer.empty()) [[likely]]
        {
            PopResT res
            {
                .iValue = std::move(iContainer.front()),
                .iFlags = iFlags
            };

            iContainer.pop();
            OnPop(uniqueLock);

            return res;
        }

        if (iFlags & FlagsT(FlagE::STOP_PRODUCE)) [[unlikely]] // STOP_PRODUCE
        {
            return PopResT
            {
                .iValue = std::nullopt,
                .iFlags = iFlags
            };
        }
    }

    // Queue is empty, wait for
    if (WaitForNextPush(aTimeout) == true) [[likely]]
    {
        return Pop();
    } else
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        return PopResT
        {
            .iValue = std::nullopt,
            .iFlags = iFlags
        };
    }
}

}// namespace GPlatform
