#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Strings/GpStringOps.hpp"
#include "../../GpUtils/Types/Strings/GpStringUtils.hpp"
#include "../../GpUtils/SyncPrimitives/GpSpinlock.hpp"

#include "GpItcSharedFuture.hpp"

namespace GPlatform {

template<typename T>
class GpItcSharedPromise
{
public:
    CLASS_DD(GpItcSharedPromise<T>)

    using value_type        = T;
    using ItcSharedFutureT  = GpItcSharedFuture<T>;
    using ItcResultT        = GpItcResult<T>;

public:
                                    GpItcSharedPromise  (void);
                                    GpItcSharedPromise  (const GpItcSharedPromise& aPromise);
                                    GpItcSharedPromise  (GpItcSharedPromise&& aPromise) noexcept;
                                    ~GpItcSharedPromise (void) noexcept;

    bool                            Fulfill             (const typename ItcResultT::EmptyValue&);
    bool                            Fulfill             (const T& aResult);
    bool                            Fulfill             (T&& aResult);
    bool                            Fulfill             (const GpException& aException);
    bool                            Fulfill             (GpException&& aException);
    bool                            Fulfill             (ItcResultT&& aItcResult);
    typename ItcSharedFutureT::SP   Future              (void);

private:
    mutable GpSpinlock              iLock;
    typename ItcSharedFutureT::SP   iFuture;
};

template<typename T>
GpItcSharedPromise<T>::GpItcSharedPromise (void)
{
}

template<typename T>
GpItcSharedPromise<T>::GpItcSharedPromise (const GpItcSharedPromise& aPromise):
iFuture(aPromise.iFuture)
{
}

template<typename T>
GpItcSharedPromise<T>::GpItcSharedPromise (GpItcSharedPromise&& aPromise) noexcept:
iFuture(std::move(aPromise.iFuture))
{
}

template<typename T>
GpItcSharedPromise<T>::~GpItcSharedPromise (void) noexcept
{
    try
    {
        std::scoped_lock lock(iLock);

        if (iFuture.IsNotNULL())
        {
            iFuture.Vn().SetEmptyResult();
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpItcSharedPromise::~GpItcSharedPromise]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpItcSharedPromise::~GpItcSharedPromise]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpItcSharedPromise::~GpItcSharedPromise]: unknown exception"_sv);
    }
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (const typename ItcResultT::EmptyValue&)
{
    return Fulfill(ItcResultT());
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (const T& aResult)
{
    return Fulfill(ItcResultT(aResult));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (T&& aResult)
{
    return Fulfill(ItcResultT(std::move(aResult)));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (const GpException& aException)
{
    return Fulfill(ItcResultT(aException));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (GpException&& aException)
{
    return Fulfill(ItcResultT(std::move(aException)));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (ItcResultT&& aItcResult)
{
    std::scoped_lock lock(iLock);

    if (iFuture.IsNotNULL())
    {
        return iFuture.Vn().SetResult(std::move(aItcResult));
    } else
    {
        return false;
    }
}

template<typename T>
typename GpItcSharedPromise<T>::ItcSharedFutureT::SP    GpItcSharedPromise<T>::Future (void)
{
    std::scoped_lock lock(iLock);

    if (iFuture.IsNULL())
    {
        iFuture = MakeSP<ItcSharedFutureT>();
    }

    return iFuture;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
