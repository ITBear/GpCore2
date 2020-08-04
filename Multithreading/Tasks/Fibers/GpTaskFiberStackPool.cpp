#include "GpTaskFiberStackPool.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberStackPool::GpTaskFiberStackPool (void) noexcept
{
}

GpTaskFiberStackPool::~GpTaskFiberStackPool (void) noexcept
{
}

void    GpTaskFiberStackPool::PreInit (const count_t /*aCount*/)
{
    //NOP
}

GpTaskFiberStackPool::value_type    GpTaskFiberStackPool::NewElement (void)
{
    auto s = GpTaskFiberStack::SP::SNew();
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