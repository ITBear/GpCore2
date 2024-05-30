#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

GP_WARNING_PUSH()
//GP_WARNING_DISABLE(shorten-64-to-32)

#include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include "../../GpTaskEnums.hpp"
#include "../GpTaskFiberCtx.hpp"
#include "GpStackBoost.hpp"

namespace GPlatform {

class GpTaskFiberCtxBoost final: public GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberCtxBoost)
    CLASS_DD(GpTaskFiberCtxBoost)

    using FiberT        = boost::context::fiber;
    using FiberRefOptT  = std::optional<std::reference_wrapper<FiberT>>;

public:
                                        GpTaskFiberCtxBoost     (void) noexcept;
    virtual                             ~GpTaskFiberCtxBoost    (void) noexcept override final;

    virtual GpException::C::Opt         Clear                   (void) noexcept override final;
    virtual GpTaskRunRes::EnumT         Enter                   (GpTaskFiber& aTaskFiber) override final;
    virtual void                        CallYield               (const GpTaskRunRes::EnumT aRunRes) override final;
    virtual void                        CallYield               (const milliseconds_t aTimeout) override final;

private:
    static boost::context::fiber        SFiberFn                (boost::context::fiber&&    aFiber,
                                                                 GpTaskFiberCtxBoost&       aFiberCtx);

private:
    boost::context::fiber               iFiber;

    GpTaskRunRes::EnumT                 iYieldRes   = GpTaskRunRes::DONE;
    GpStackBoost::StackImplT            iStack;
    GpTaskFiber*                        iTaskFiber  = nullptr;
    std::optional<std::exception_ptr>   iException;
    bool                                iIsCallStopInProgress = false;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
