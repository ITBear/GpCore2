#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStack.hpp"

namespace GPlatform {

class GpTaskFiberStackPool final: public GpElementsPool<GpTaskFiberStack::SP>
{
public:
                                GpTaskFiberStackPool    (void) noexcept = default;
    virtual                     ~GpTaskFiberStackPool   (void) noexcept override final = default;

    void                        SetStackSize            (const size_byte_t aStackSize) noexcept {iStackSize = aStackSize;}

protected:
    inline virtual void         PreInit                 (const size_t aCount) override final;
    inline virtual value_type   NewElement              (GpSpinlock& aLocked) override final;
    inline virtual void         OnClear                 (void) noexcept override final;

private:
    size_byte_t                 iStackSize = 0_byte;
};

void    GpTaskFiberStackPool::PreInit (const size_t /*aCount*/)
{
    //NOP
}

GpTaskFiberStackPool::value_type    GpTaskFiberStackPool::NewElement (GpSpinlock& /*aLocked*/)
{
    auto s = MakeSP<GpTaskFiberStack>();
    s.Vn().Init(iStackSize);
    return s;
}

void    GpTaskFiberStackPool::OnClear (void) noexcept
{
    //NOP
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
