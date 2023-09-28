#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcSharedPromise.hpp"
#include "GpItcSharedFuture.hpp"

namespace GPlatform {

template<typename T>
class GpItcSharedPromiseHolder
{
public:
    using value_t   = T;
    using PromiseT  = GpItcSharedPromise<T>;
    using FutureT   = GpItcSharedFuture<T>;

public:
                            GpItcSharedPromiseHolder    (void) noexcept = default;
                            ~GpItcSharedPromiseHolder   (void) noexcept = default;

    typename FutureT::SP    Future                      (void);

    bool                    Fulfill                     (void) noexcept;
    bool                    Fulfill                     (const std::exception&  aException) noexcept;
    bool                    Fulfill                     (const GpException&     aException) noexcept;
    bool                    Fulfill                     (typename PromiseT::value_type&& aResult) noexcept;
    bool                    Fulfill                     (typename PromiseT::ItcResultT&& aResult) noexcept;

private:
    PromiseT                iPromise;
    bool                    iIsFulfilled    = false;
};

template<typename T>
typename GpItcSharedPromiseHolder<T>::FutureT::SP   GpItcSharedPromiseHolder<T>::Future (void)
{
    return iPromise.Future();
}

template<typename T>
bool    GpItcSharedPromiseHolder<T>::Fulfill (void) noexcept
{
    return Fulfill(typename PromiseT::ItcResultT());
}

template<typename T>
bool    GpItcSharedPromiseHolder<T>::Fulfill (const GpException& aException) noexcept
{
    return Fulfill(typename PromiseT::ItcResultT(aException));
}

template<typename T>
bool    GpItcSharedPromiseHolder<T>::Fulfill (typename PromiseT::value_type&& aResult) noexcept
{
    return Fulfill(typename PromiseT::ItcResultT(std::move(aResult)));
}

template<typename T>
bool    GpItcSharedPromiseHolder<T>::Fulfill (typename PromiseT::ItcResultT&& aResult) noexcept
{
    if (iIsFulfilled)
    {
        return true;
    }

    try
    {
        iIsFulfilled = true;
        iPromise.Fulfill(std::move(aResult));
        return true;
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpItcSharedPromiseHolder::Fulfill]: unknown exception"_sv);
    }

    return false;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
