#include "GpTaskFiberStack.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "boost_context.hpp"

#include <iostream>

namespace GPlatform {

static int _GpTaskFiberStack_counter = 0;

GpTaskFiberStack::GpTaskFiberStack (void) noexcept
{
    _GpTaskFiberStack_counter++;
    std::cout << "[GpTaskFiberStack::GpTaskFiberStack]: counter = " << _GpTaskFiberStack_counter << std::endl;
}

GpTaskFiberStack::~GpTaskFiberStack (void) noexcept
{
    Clear();

    _GpTaskFiberStack_counter--;
    std::cout << "[GpTaskFiberStack::~GpTaskFiberStack]: counter = " << _GpTaskFiberStack_counter << std::endl;
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
