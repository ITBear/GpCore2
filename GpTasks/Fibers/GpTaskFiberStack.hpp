#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "boost_context.hpp"

namespace GPlatform {

class GpTaskFiberStack
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberStack)
    CLASS_DD(GpTaskFiberStack)

public:
    inline              GpTaskFiberStack    (void) noexcept;
    inline              ~GpTaskFiberStack   (void) noexcept;

    inline void         Init                (const size_byte_t aSize);
    size_byte_t         Size                (void) const noexcept {return iSize;}
    inline void         Clear               (void) noexcept;
    void*               Stack               (void) noexcept {return iStack;}

private:
    void*               iStack  = nullptr;//GpFiberStackT*
    size_byte_t         iSize   = 0_byte;
};

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
