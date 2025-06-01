#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringLiterals.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

#include <mutex>
#include <queue>
#include <optional>

namespace GPlatform {

// TODO: Reimplement like GpCachePoolMap

template<typename T>
class GpSharedPool
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSharedPool)

public:
    using this_type     = GpSharedPool<T>;
    using value_type    = T;
    using QueueT        = std::queue<value_type>;

    TAG_SET(THREAD_SAFE)

    enum class ReleaseAction
    {
        PUSH_TO_ELEMENTS,
        ACQUIRED
    };

public:
                                        GpSharedPool            (void) noexcept;
    virtual                             ~GpSharedPool           (void) noexcept;

    void                                Init                    (size_t aInitCount,
                                                                 size_t aMaxCount);
    void                                Clear                   (void) noexcept;

    std::optional<value_type>           Acquire                 (void);
    void                                Release                 (value_type&& aElement);

    size_t                              InitCount               (void) const noexcept;
    size_t                              MaxCount                (void) const noexcept;
    size_t                              AcquiredCount           (void) const noexcept;
    bool                                IsInit                  (void) const noexcept;

protected:
    virtual value_type                  NewElement              (void);
    virtual bool                        Validate                (value_type& aElement) noexcept;

    virtual void                        OnAcquire               (value_type& aValue);
    virtual ReleaseAction               OnRelease               (value_type& aValue);
    virtual std::optional<value_type>   OnAcquireNoElementsLeft (void);

protected:
    mutable GpSpinLockRW                iSpinLockRW;

private:
    QueueT                              iElements       GUARDED_BY(iSpinLockRW);
    size_t                              iInitCount      GUARDED_BY(iSpinLockRW) = {0};
    size_t                              iMaxCount       GUARDED_BY(iSpinLockRW) = {0};
    size_t                              iAcquiredCount  GUARDED_BY(iSpinLockRW) = {0};
    bool                                iIsInit         GUARDED_BY(iSpinLockRW) = false;
};

template<typename T>
GpSharedPool<T>::GpSharedPool (void) noexcept
{
}

template<typename T>
GpSharedPool<T>::~GpSharedPool (void) noexcept
{
    Clear();
}

template<typename T>
void    GpSharedPool<T>::Init
(
    const size_t aInitCount,
    const size_t aMaxCount
)
{
    VERIFY
    (
        aMaxCount >= aInitCount,
        "aMaxCount >= aInitCount"_sv
    );

    Clear();

    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    VERIFY
    (
        iIsInit == false,
        "Already initialized"_sv
    );

    for (size_t i = 0; i < aInitCount; i++)
    {
        iElements.push(NewElement());
    }

    iInitCount      = aInitCount;
    iMaxCount       = aMaxCount;
    iAcquiredCount  = 0;
    iIsInit         = true;
}

template<typename T>
void    GpSharedPool<T>::Clear (void) noexcept
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    while (!iElements.empty())
    {
        iElements.pop();
    }

    iInitCount      = {0};
    iMaxCount       = {0};
    iAcquiredCount  = {0};
    iIsInit         = false;
}

template<typename T>
auto    GpSharedPool<T>::Acquire (void) -> std::optional<value_type>
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (iElements.empty())
    {
        if (iAcquiredCount < iMaxCount)
        {
            value_type e = NewElement();
            OnAcquire(e);
            iAcquiredCount++;
            return e;
        } else
        {
            std::optional<value_type> op_e = OnAcquireNoElementsLeft();

            if (op_e.has_value())
            {
                OnAcquire(op_e.value());
                iAcquiredCount++;
            }

            return op_e;
        }
    } else
    {
        value_type e = std::move(iElements.front());
        iElements.pop();
        OnAcquire(e);
        iAcquiredCount++;

        return e;
    }
}

template<typename T>
void    GpSharedPool<T>::Release (value_type&& aElement)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    VERIFY
    (
        iAcquiredCount > 0,
        "Release without acquire"_sv
    );

    iAcquiredCount--;

    if (Validate(aElement))
    {
        if (OnRelease(aElement) == ReleaseAction::PUSH_TO_ELEMENTS)
        {
            iElements.push(std::move(aElement));
        }
    }
}

template<typename T>
size_t  GpSharedPool<T>::InitCount (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return iInitCount;
}

template<typename T>
size_t  GpSharedPool<T>::MaxCount (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return iMaxCount;
}

template<typename T>
size_t  GpSharedPool<T>::AcquiredCount (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return iAcquiredCount;
}

template<typename T>
bool    GpSharedPool<T>::IsInit (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return iIsInit;
}

template<typename T>
auto    GpSharedPool<T>::NewElement (void) -> value_type
{
    return T{};
}

template<typename T>
bool    GpSharedPool<T>::Validate (value_type& /*aElement*/) noexcept
{
    return true;
}

template<typename T>
void    GpSharedPool<T>::OnAcquire
(
    value_type& /*aValue*/
)
{
    //NOP
}

template<typename T>
auto    GpSharedPool<T>::OnRelease (value_type&) -> ReleaseAction
{
    return ReleaseAction::PUSH_TO_ELEMENTS;
}

template<typename T>
auto    GpSharedPool<T>::OnAcquireNoElementsLeft (void) -> std::optional<value_type>
{
    return std::nullopt;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
