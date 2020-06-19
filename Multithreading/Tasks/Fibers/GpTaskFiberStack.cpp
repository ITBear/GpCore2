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

    iStackAllocator = GpMemOps::SNew<BasicProtectedFixedSizeStackT>(aSize.ValueAs<size_t>());
    iStackContext   = GpMemOps::SNew<StackContextT>(reinterpret_cast<BasicProtectedFixedSizeStackT*>(iStackAllocator)->allocate());
}

void    GpTaskFiberStack::Clear (void) noexcept
{
    if (iStackAllocator != nullptr)
    {
        BasicProtectedFixedSizeStackT* allocator = reinterpret_cast<BasicProtectedFixedSizeStackT*>(iStackAllocator);

        if (iStackContext != nullptr)
        {
            StackContextT* sctx = reinterpret_cast<StackContextT*>(iStackContext);

            allocator->deallocate(*sctx);

            GpMemOps::SDelete(sctx);
            iStackContext = nullptr;
        }

        GpMemOps::SDelete(allocator);
        iStackAllocator = nullptr;
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
