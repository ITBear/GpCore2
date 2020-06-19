#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStack.hpp"
#include "../../../Types/Containers/GpElementsPool.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberStackPool: public GpElementsPool<GpTaskFiberStack::SP>
{
public:
                            GpTaskFiberStackPool    (void) noexcept;
    virtual                 ~GpTaskFiberStackPool   (void) noexcept override final;

    void                    SetStackSize            (const size_byte_t aStackSize) noexcept {iStackSize = aStackSize;}

protected:
    virtual void            PreInit                 (const count_t aCount) override final;
    virtual value_type      NewElement              (void) override final;
    virtual void            OnClear                 (void) noexcept override final;

private:
    size_byte_t             iStackSize = 0_byte;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
