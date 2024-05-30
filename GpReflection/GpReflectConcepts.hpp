#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectObject.hpp"

namespace GPlatform::Concepts {

template <typename T>
concept ReflectObject = requires()
{
    requires GpHasTag_GpReflectObject<T>() == true;
};

}// namespace GPlatform::Concepts
