#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../../SyncPrimitives/GpSpinlock.hpp"
#include "../../Macro/GpMacroTags.hpp"
#include "../../Types/Strings/GpStringLiterals.hpp"
#include "../../Exceptions/GpException.hpp"

#include <mutex>
#include <queue>
#include <optional>

namespace GPlatform {

template<typename T>
class GpElementsPool
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpElementsPool)

public:
    using this_type     = GpElementsPool<T>;
    using value_type    = T;
    using QueueT        = std::queue<value_type>;

    TAG_SET(THREAD_SAFE)

    enum class ReleaseAct
    {
        PUSH_TO_ELEMENTS,
        ACQUIRED
    };

public:
                                        GpElementsPool          (void) noexcept;
    virtual                             ~GpElementsPool         (void) noexcept;

    void                                Init                    (const size_t aInitCount,
                                                                 const size_t aMaxCount);
    void                                Clear                   (void) noexcept;

    std::optional<value_type>           Acquire                 (void);
    void                                Release                 (value_type&& aElement);

    size_t                              InitCount               (void) const noexcept;
    size_t                              MaxCount                (void) const noexcept;
    size_t                              AcquiredCount           (void) const noexcept;
    bool                                IsInit                  (void) const noexcept;

protected:
    virtual void                        PreInit                 (const size_t aCount);
    virtual value_type                  NewElement              (GpSpinlock& aLocked);
    virtual void                        OnClear                 (void) noexcept;
    virtual bool                        Validate                (value_type& aElement) noexcept;

    virtual void                        OnAcquire               (value_type& aValue,
                                                                 GpSpinlock& aLocked);
    virtual ReleaseAct                  OnRelease               (value_type& aValue,
                                                                 GpSpinlock& aLocked);
    virtual std::optional<value_type>   OnAcquireNoElementsLeft (GpSpinlock& aLocked);

private:
    void                                _Clear                  (bool aIsDestructorCall) noexcept;

protected:
    mutable GpSpinlock                  iLock;

private:
    QueueT                              iElements;
    size_t                              iInitCount      = 0;
    size_t                              iMaxCount       = 0;
    size_t                              iAcquiredCount  = 0;
    bool                                iIsInit         = false;
};

template<typename T>
GpElementsPool<T>::GpElementsPool (void) noexcept
{
}

template<typename T>
GpElementsPool<T>::~GpElementsPool (void) noexcept
{
    _Clear(true);
}

template<typename T>
void    GpElementsPool<T>::Init
(
    const size_t aInitCount,
    const size_t aMaxCount
)
{
    THROW_COND_GP
    (
        aMaxCount >= aInitCount,
        u8"aMaxCount >= aInitCount"_sv
    );

    Clear();

    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iIsInit == false,
        u8"Already initialized"_sv
    );

    PreInit(aInitCount);

    for (size_t i = 0; i < aInitCount; i++)
    {
        iElements.push(NewElement(iLock));
    }

    iInitCount      = aInitCount;
    iMaxCount       = aMaxCount;
    iAcquiredCount  = 0;
    iIsInit         = true;
}

template<typename T>
void    GpElementsPool<T>::Clear (void) noexcept
{
    _Clear(false);
}

template<typename T>
typename std::optional<typename GpElementsPool<T>::value_type>  GpElementsPool<T>::Acquire (void)
{
    std::scoped_lock lock(iLock);

    if (iElements.empty())
    {
        if (iAcquiredCount < iMaxCount)
        {
            value_type e = NewElement(iLock);
            OnAcquire(e, iLock);
            iAcquiredCount++;
            return e;
        } else
        {
            std::optional<value_type> op_e = OnAcquireNoElementsLeft(iLock);

            if (op_e.has_value())
            {
                OnAcquire(op_e.value(), iLock);
                iAcquiredCount++;
            }

            return op_e;
        }
    } else
    {
        value_type e = std::move(iElements.front());
        iElements.pop();
        OnAcquire(e, iLock);
        iAcquiredCount++;

        return e;
    }
}

template<typename T>
void    GpElementsPool<T>::Release (value_type&& aElement)
{
    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iAcquiredCount > 0,
        u8"Release without acquire"_sv
    );

    iAcquiredCount--;

    if (Validate(aElement))
    {
        if (OnRelease(aElement, iLock) == ReleaseAct::PUSH_TO_ELEMENTS)
        {
            iElements.push(std::move(aElement));
        }
    }
}

template<typename T>
size_t  GpElementsPool<T>::InitCount (void) const noexcept
{
    std::scoped_lock lock(iLock);
    return iInitCount;
}

template<typename T>
size_t  GpElementsPool<T>::MaxCount (void) const noexcept
{
    std::scoped_lock lock(iLock);
    return iMaxCount;
}

template<typename T>
size_t  GpElementsPool<T>::AcquiredCount (void) const noexcept
{
    std::scoped_lock lock(iLock);
    return iAcquiredCount;
}

template<typename T>
bool    GpElementsPool<T>::IsInit (void) const noexcept
{
    std::scoped_lock lock(iLock);
    return iIsInit;
}

template<typename T>
void    GpElementsPool<T>::PreInit (const size_t /*aCount*/)
{
    //NOP
}

template<typename T>
typename GpElementsPool<T>::value_type  GpElementsPool<T>::NewElement (GpSpinlock& /*aLocked*/)
{
    return T();
}

template<typename T>
void    GpElementsPool<T>::OnClear (void) noexcept
{
    //NOP
}

template<typename T>
bool    GpElementsPool<T>::Validate (value_type& /*aElement*/) noexcept
{
    return true;
}

template<typename T>
void    GpElementsPool<T>::OnAcquire
(
    value_type& /*aValue*/,
    GpSpinlock& /*aLocked*/
)
{
    //NOP
}

template<typename T>
typename GpElementsPool<T>::ReleaseAct  GpElementsPool<T>::OnRelease
(
    value_type& /*aValue*/,
    GpSpinlock& /*aLocked*/
)
{
    return ReleaseAct::PUSH_TO_ELEMENTS;
}

template<typename T>
typename std::optional<typename GpElementsPool<T>::value_type>  GpElementsPool<T>::OnAcquireNoElementsLeft (GpSpinlock& /*aLocked*/)
{
    return std::nullopt;
}

template<typename T>
void    GpElementsPool<T>::_Clear (bool aIsDestructorCall) noexcept
{
    std::scoped_lock lock(iLock);

    if (aIsDestructorCall == false)
    {
        OnClear();
    }

    while (iElements.empty() == false)
    {
        iElements.pop();
    }

    iInitCount      = 0;
    iMaxCount       = 0;
    iAcquiredCount  = 0;
    iIsInit         = false;
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
