#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFuture.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template<typename T>
class GpItcPromise
{
public:
    CLASS_DD(GpItcPromise<T>)
    TAG_SET(THREAD_SAFE)

    using value_type    = T;
    using FutureT       = GpItcFuture<T>;
    using ResultT       = GpItcResult<T>;

public:
                            GpItcPromise    (void);
                            GpItcPromise    (const GpItcPromise& aPromise);
                            GpItcPromise    (GpItcPromise&& aPromise) noexcept;
                            ~GpItcPromise   (void) noexcept;

    GpItcPromise&           operator=       (const GpItcPromise& aPromise);
    GpItcPromise&           operator=       (GpItcPromise&& aPromise) noexcept;

    bool                    Fulfill         (const T& aResult);
    bool                    Fulfill         (T&& aResult);
    bool                    Fulfill         (const GpException& aException);
    bool                    Fulfill         (GpException&& aException);
    bool                    Fulfill         (ResultT&& aResult);

    typename FutureT::SP    Future          (void);

private:
    typename FutureT::SP    iFuture;
};

template<typename T>
GpItcPromise<T>::GpItcPromise (void):
iFuture{MakeSP<FutureT>()}
{
}

template<typename T>
GpItcPromise<T>::GpItcPromise (const GpItcPromise& aPromise):
iFuture{aPromise.iFuture}
{
}

template<typename T>
GpItcPromise<T>::GpItcPromise (GpItcPromise&& aPromise) noexcept:
iFuture{std::move(aPromise.iFuture)}
{
}

template<typename T>
GpItcPromise<T>::~GpItcPromise (void) noexcept
{
    try
    {
        if (   iFuture.IsNotNULL()
            && (iFuture.Vn().IsReady() == false))
        {
            Fulfill(GpException{"Empty result"});
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpItcPromise::~GpItcPromise]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpItcPromise::~GpItcPromise]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpItcPromise::~GpItcPromise]: unknown exception"_sv);
    }
}

template<typename T>
GpItcPromise<T>&    GpItcPromise<T>::operator= (const GpItcPromise& aPromise)
{
    iFuture = aPromise.iFuture;

    return *this;
}

template<typename T>
GpItcPromise<T>&    GpItcPromise<T>::operator= (GpItcPromise&& aPromise) noexcept
{
    iFuture = std::move(aPromise.iFuture);

    return *this;
}

template<typename T>
bool    GpItcPromise<T>::Fulfill (const T& aResult)
{
    return Fulfill(ResultT{aResult}  );
}

template<typename T>
bool    GpItcPromise<T>::Fulfill (T&& aResult)
{
    return Fulfill(ResultT{std::move(aResult)});
}

template<typename T>
bool    GpItcPromise<T>::Fulfill (const GpException& aException)
{
    return Fulfill(ResultT{aException});
}

template<typename T>
bool    GpItcPromise<T>::Fulfill (GpException&& aException)
{
    return Fulfill(ResultT{std::move(aException)});
}

template<typename T>
bool    GpItcPromise<T>::Fulfill (ResultT&& aResult)
{
    if (iFuture.IsNotNULL())
    {
        return iFuture.Vn().SetResult
        (
            std::move(aResult),
            GpMethodAccess{this}
        );
    } else
    {
        return false;
    }
}

template<typename T>
typename GpItcPromise<T>::FutureT::SP   GpItcPromise<T>::Future (void)
{
    return iFuture;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
