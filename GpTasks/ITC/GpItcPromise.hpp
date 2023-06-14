#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../GpUtils/Macro/GpMacroWarnings.hpp"
#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Strings/GpStringOps.hpp"
#include "../../GpUtils/Types/Strings/GpStringUtils.hpp"

GP_WARNING_PUSH()
GP_WARNING_DISABLE(shadow)

//#include "../../GpUtils/TypeTraits/GpTypeTraitsResultOf.hpp"
#include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include "GpItcFuture.hpp"

namespace GPlatform {

template<typename T>
class GpItcPromise
{
public:
    CLASS_REMOVE_CTRS_COPY(GpItcPromise)
    CLASS_DD(GpItcPromise<T>)

    using ItcFutureT = GpItcFuture<T>;
    using ItcResultT = GpItcResult<T>;

public:
    inline                          GpItcPromise    (void);
    inline                          GpItcPromise    (this_type&& aPromise) noexcept;
    inline                          ~GpItcPromise   (void) noexcept;

    inline void                     Complete        (typename ItcResultT::SP aResult);
    inline typename ItcFutureT::SP  Future          (const GpUUID& aTaskGuid);

private:
    typename ItcFutureT::SP         iFuture;
};

template<typename T>
GpItcPromise<T>::GpItcPromise (void):
iFuture(MakeSP<ItcFutureT>())
{
}

template<typename T>
GpItcPromise<T>::GpItcPromise (this_type&& aPromise) noexcept:
iFuture(std::move(aPromise.iFuture))
{
}

template<typename T>
GpItcPromise<T>::~GpItcPromise (void) noexcept
{
    try
    {
        Complete(ItcResultT::SP::SNull());
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::~GpItcPromise]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::~GpItcPromise]: exception: "_sv + e.what());
    } catch (const boost::context::detail::forced_unwind&)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::~GpItcPromise]: exception: boost::context::detail::forced_unwind"_sv);
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::~GpItcPromise]: unknown exception"_sv);
    }
}

template<typename T>
void    GpItcPromise<T>::Complete (typename ItcResultT::SP aResult)
{
    if (iFuture.IsNULL())
    {
        return;
    }

    try
    {
        iFuture.Vn().SetResultOnce(std::move(aResult));
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::Complete]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::Complete]: exception: "_sv + e.what());
    } catch (const boost::context::detail::forced_unwind&)
    {
        throw;
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpItcPromise::Complete]: unknown exception"_sv);
    }
}

template<typename T>
typename GpItcPromise<T>::ItcFutureT::SP    GpItcPromise<T>::Future (const GpUUID& aTaskGuid)
{
    iFuture.V().AddTaskGuid(aTaskGuid);
    return iFuture;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
