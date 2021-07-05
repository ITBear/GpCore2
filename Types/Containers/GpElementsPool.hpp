#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../Units/Other/count_t.hpp"
#include "../../Multithreading/SyncPrimitives/GpSpinlock.hpp"

#include <mutex>

namespace GPlatform {

template<typename T>
class GpElementsPool
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpElementsPool)

public:
    using this_type     = GpElementsPool<T>;
    using value_type    = T;
    using QueueT        = GpQueue<value_type>;

    CLASS_TAG(THREAD_SAFE)

    enum class ReleaseAct
    {
        PUSH_TO_ELEMENTS,
        ACQUIRED
    };

public:
                                        GpElementsPool          (void) noexcept;
    virtual                             ~GpElementsPool         (void) noexcept;

    count_t                             MaxCount                (void) const noexcept {return iMaxCount;}

    void                                Init                    (const count_t aInitCount,
                                                                 const count_t aMaxCount);
    void                                Clear                   (void) noexcept;

    std::optional<value_type>           Acquire                 (void);
    void                                Release                 (value_type aElement);

protected:
    virtual void                        PreInit                 (const count_t aCount);
    virtual value_type                  NewElement              (GpSpinlock& aLocked);
    virtual void                        OnClear                 (void) noexcept;
    virtual bool                        Validate                (value_type aElement) noexcept;

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
    count_t                             iInitCount      = 0_cnt;
    count_t                             iMaxCount       = 0_cnt;
    count_t                             iAcquiredCount  = 0_cnt;
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
    const count_t aInitCount,
    const count_t aMaxCount
)
{
    THROW_GPE_COND(aInitCount >= 0_cnt, "aInitCount >= 0_cnt"_sv);
    THROW_GPE_COND(aMaxCount >= aInitCount, "aMaxCount >= aInitCount"_sv);

    Clear();

    std::scoped_lock lock(iLock);

    THROW_GPE_COND
    (
        iIsInit == false,
        "Already initialized"_sv
    );

    PreInit(aInitCount);

    for (count_t i = 0_cnt; i < aInitCount; i++)
    {
        iElements.push(NewElement(iLock));
    }

    iInitCount      = aInitCount;
    iMaxCount       = aMaxCount;
    iAcquiredCount  = 0_cnt;
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

    if (iElements.size() == 0)
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
        value_type e = iElements.front();
        iElements.pop();
        OnAcquire(e, iLock);
        iAcquiredCount++;

        return e;
    }
}

template<typename T>
void    GpElementsPool<T>::Release (value_type aElement)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND
    (
        iAcquiredCount > 0_cnt,
        "Release without acquire"_sv
    );

    iAcquiredCount--;

    if (Validate(aElement))
    {
        if (OnRelease(aElement, iLock) == ReleaseAct::PUSH_TO_ELEMENTS)
        {
            iElements.push(aElement);
        }
    }
}

template<typename T>
void    GpElementsPool<T>::PreInit (const count_t /*aCount*/)
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
bool    GpElementsPool<T>::Validate (value_type /*aElement*/) noexcept
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

    iInitCount      = 0_cnt;
    iMaxCount       = 0_cnt;
    iAcquiredCount  = 0_cnt;
    iIsInit         = false;
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
