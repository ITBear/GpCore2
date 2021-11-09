#include "GpTaskFiberStack.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "boost_context.hpp"

namespace GPlatform {

GpTaskFiberStack::GpTaskFiberStack (void) noexcept
{
}

GpTaskFiberStack::~GpTaskFiberStack (void) noexcept
{
    Clear();
}

void    GpTaskFiberStack::Init (const size_byte_t aSize)
{
    Clear();
    iStack  = GpMemOps::SNew<GpFiberStackT>(aSize.As<size_t>());
    iSize   = aSize;
}

void    GpTaskFiberStack::Clear (void) noexcept
{
    if (iStack != nullptr)
    {
        GpFiberStackT* stack = reinterpret_cast<GpFiberStackT*>(iStack);
        GpMemOps::SDelete(stack);
        iStack  = nullptr;
        iSize   = 0_byte;
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
