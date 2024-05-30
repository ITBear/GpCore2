#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>

#include "GpItcSharedFuture.hpp"

namespace GPlatform {

template<typename T>
class GpItcSharedPromise
{
public:
    CLASS_DD(GpItcSharedPromise<T>)
    TAG_SET(THREAD_SAFE)

    using value_type    = T;
    using FutureT       = GpItcSharedFuture<T>;
    using ResultT       = GpItcResult<T>;

public:
                            GpItcSharedPromise  (void);
                            GpItcSharedPromise  (const GpItcSharedPromise& aPromise);
                            GpItcSharedPromise  (GpItcSharedPromise&& aPromise) noexcept;
                            ~GpItcSharedPromise (void) noexcept;

    bool                    Fulfill             (const T& aResult);
    bool                    Fulfill             (T&& aResult);
    bool                    Fulfill             (const GpException& aException);
    bool                    Fulfill             (GpException&& aException);
    bool                    Fulfill             (ResultT&& aResult);

    typename FutureT::SP    Future              (void);

private:
    typename FutureT::SP    iFuture;
};

template<typename T>
GpItcSharedPromise<T>::GpItcSharedPromise (void):
iFuture(MakeSP<FutureT>())
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
        if (iFuture.IsNotNULL())
        {
            Fulfill(GpException("Empty result"));
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpItcSharedPromise::~GpItcSharedPromise]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpItcSharedPromise::~GpItcSharedPromise]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpItcSharedPromise::~GpItcSharedPromise]: unknown exception"_sv);
    }
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (const T& aResult)
{
    return Fulfill(ResultT(aResult));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (T&& aResult)
{
    return Fulfill(ResultT(std::move(aResult)));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (const GpException& aException)
{
    return Fulfill(ResultT(aException));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (GpException&& aException)
{
    return Fulfill(ResultT(std::move(aException)));
}

template<typename T>
bool    GpItcSharedPromise<T>::Fulfill (ResultT&& aResult)
{
    if (iFuture.IsNotNULL())
    {
        return iFuture.Vn().SetResult(std::move(aResult));
    } else
    {
        return false;
    }
}

template<typename T>
typename GpItcSharedPromise<T>::FutureT::SP GpItcSharedPromise<T>::Future (void)
{
    return iFuture;
}

}// namespace GPlatform
