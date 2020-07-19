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

public:
                                GpElementsPool  (void) noexcept;
    virtual                     ~GpElementsPool (void) noexcept;

    void                        Init            (const count_t aInitCount,
                                                 const count_t aMaxCount);
    void                        Clear           (void) noexcept;

    std::optional<value_type>   Accuire         (void);
    void                        Release         (value_type aElement);

protected:
    virtual void                PreInit         (const count_t aCount);
    virtual value_type          NewElement      (void);
    virtual void                OnClear         (void) noexcept;

private:
    void                        _Clear          (bool aIsDestructorCall) noexcept;

protected:
    mutable GpSpinlock          iLock;

private:
    QueueT                      iElements;
    count_t                     iInitCount      = 0_cnt;
    count_t                     iMaxCount       = 0_cnt;
    count_t                     iAccuiredCount  = 0_cnt;
    bool                        iIsInit         = false;
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
void    GpElementsPool<T>::Init (const count_t aInitCount,
                                 const count_t aMaxCount)
{
    THROW_GPE_COND_CHECK(aInitCount >= 0_cnt);
    THROW_GPE_COND_CHECK(aMaxCount >= aInitCount);

    Clear();

    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(iIsInit == false, "Already initialized"_sv);

    PreInit(aInitCount);

    for (count_t i = 0_cnt; i < aInitCount; i++)
    {
        iElements.push(NewElement());
    }

    iInitCount      = aInitCount;
    iMaxCount       = aMaxCount;
    iAccuiredCount  = 0_cnt;
    iIsInit         = true;
}

template<typename T>
void    GpElementsPool<T>::Clear (void) noexcept
{
    _Clear(false);
}

template<typename T>
typename std::optional<typename GpElementsPool<T>::value_type>  GpElementsPool<T>::Accuire (void)
{
    std::scoped_lock lock(iLock);

    if (iElements.size() == 0)
    {
        if (iAccuiredCount < iMaxCount)
        {
            value_type e = NewElement();
            iAccuiredCount++;
            return e;
        } else
        {
            return std::nullopt;
        }
    }

    value_type e = iElements.front();
    iElements.pop();
    iAccuiredCount++;

    return e;
}

template<typename T>
void    GpElementsPool<T>::Release (value_type aElement)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(iAccuiredCount > 0_cnt, "Release without accuire"_sv);

    iElements.push(aElement);
    iAccuiredCount--;
}

template<typename T>
void    GpElementsPool<T>::PreInit (const count_t /*aCount*/)
{
    //NOP
}

template<typename T>
typename GpElementsPool<T>::value_type  GpElementsPool<T>::NewElement (void)
{
    return T();
}

template<typename T>
void    GpElementsPool<T>::OnClear (void) noexcept
{
    //NOP
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
    iAccuiredCount  = 0_cnt;
    iIsInit         = false;
}

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
