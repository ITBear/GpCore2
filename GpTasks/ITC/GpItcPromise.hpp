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
    inline                  GpItcPromise    (void);
    inline                  GpItcPromise    (GpItcPromise&& aPromise) noexcept;
    inline                  ~GpItcPromise   (void) noexcept;

    inline void             Complete        (GpItcResult::SP aResult) noexcept;
    inline GpItcFuture::SP  Future          (const GpUUID& aTaskGuid);

private:
    GpItcFuture::SP         iFuture;
};

GpItcPromise::GpItcPromise (void):
iFuture(MakeSP<GpItcFuture>())
{
}

GpItcPromise::GpItcPromise (GpItcPromise&& aPromise) noexcept:
iFuture(std::move(aPromise.iFuture))
{
}

GpItcPromise::~GpItcPromise (void) noexcept
{
    Complete(GpItcResult::SP::SNull());
}

void    GpItcPromise::Complete (GpItcResult::SP aResult) noexcept
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
        GpStringUtils::SCerr("[GpItcPromise::Complete]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpItcPromise::Complete]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpItcPromise::Complete]: unknown exception"_sv);
    }
}

GpItcFuture::SP GpItcPromise::Future (const GpUUID& aTaskGuid)
{
    iFuture.V().AddTaskGuid(aTaskGuid);
    return iFuture;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
