#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTask.hpp"

namespace GPlatform {

class GpTaskFiberAccessor;
class GpTaskFiberCtx;

class GP_TASKS_API GpTaskFiber: public GpTask
{
    friend class GpTaskFiberAccessor;
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiber)
    CLASS_DD(GpTaskFiber)

    enum class StageT
    {
        NOT_RUN,
        RUN,
        FINISHED
    };

public:
                                    GpTaskFiber     (std::u8string aName) noexcept;
    virtual                         ~GpTaskFiber    (void) noexcept override;

    static void                     SYield          (const GpTaskDoRes aRes);
    static bool                     SIsIntoFiber    (void) noexcept;

protected:
    virtual void                    FiberFn         (GpThreadStopToken aStopToken) = 0;
    virtual GpTaskDoRes             _Run            (GpThreadStopToken aStopToken) noexcept override final;

    void                            ClearCtx        (void) noexcept;

private:
    GpTaskFiberCtx*                 Ctx             (void) {return iCtx.get();}
    static GpTaskFiberCtx*          SCtx            (void);

private:
    std::unique_ptr<GpTaskFiberCtx> iCtx;
    StageT                          iStage = StageT::NOT_RUN;
};

inline void YELD (const GpTaskDoRes aRes)
{
    GpTaskFiber::SYield(aRes);
}

inline void YELD_READY (void)
{
    GpTaskFiber::SYield(GpTaskDoRes::READY_TO_EXEC);
}

inline void YELD_WAITING (void)
{
    GpTaskFiber::SYield(GpTaskDoRes::WAITING);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
