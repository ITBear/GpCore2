#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcFuture.hpp"

namespace GPlatform {

class GpItcPromise
{
public:
    CLASS_REMOVE_CTRS_COPY(GpItcPromise)

public:
                            GpItcPromise    (void) noexcept = default;
    inline                  GpItcPromise    (GpItcPromise&& aPromise) noexcept;
    inline                  ~GpItcPromise   (void) noexcept;

    inline void             Complete        (GpItcResult::SP aResult) noexcept;

    inline GpItcFuture::SP  Future          (void);

private:
    mutable GpRWLock        iFutureLock;
    GpItcFuture::SP         iFuture;
};

inline  GpItcPromise::GpItcPromise (GpItcPromise&& aPromise) noexcept:
iFuture(std::move(aPromise.iFuture))
{
}

GpItcPromise::~GpItcPromise (void) noexcept
{
    std::scoped_lock lock(iFutureLock);

    if (iFuture.IsNotNULL())
    {
        iFuture.Vn().PromiseDestructed();
        iFuture.Clear();
    }
}

GpItcFuture::SP GpItcPromise::Future (void)
{
    //Quick check
    {
        std::shared_lock lock(iFutureLock);

        if (iFuture.IsNotNULL())
        {
            return iFuture;
        }
    }

    //Create future
    {
        std::scoped_lock lock(iFutureLock);

        if (iFuture.IsNotNULL())
        {
            return iFuture;
        }

        iFuture = MakeSP<GpItcFuture>();
        return iFuture;
    }
}

void    GpItcPromise::Complete (GpItcResult::SP aResult) noexcept
{
    std::scoped_lock lock(iFutureLock);

    if (iFuture.IsNotNULL())
    {
        iFuture.Vn().SetResult(std::move(aResult));
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
