#pragma once

#include "../GpUtils/GpUtils.hpp"

#if defined(GP_TASKS_LIBRARY)
    #define GP_TASKS_API GP_DECL_EXPORT
#else
    #define GP_TASKS_API GP_DECL_IMPORT
#endif
