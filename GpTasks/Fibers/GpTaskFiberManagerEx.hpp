#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

class GP_TASKS_API GpTaskFiberManagerEx
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberManagerEx)

public:
    static void         SInit   (const size_t       aMaxStacksCount,
                                 const size_byte_t  aStackSize);
    static void         SClear  (void) noexcept;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
