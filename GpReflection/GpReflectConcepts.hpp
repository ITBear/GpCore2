#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObject.hpp"

namespace GPlatform::Concepts {

template <typename T>
concept ReflectObject = requires()
{
    requires GpHasTag_GpReflectObject<T>() == true;
};

}//namespace GPlatform::Concepts

#endif//GP_USE_REFLECTION
