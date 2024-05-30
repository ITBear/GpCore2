#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpUtils/Types/Containers/GpVectorWrapper.hpp>

namespace GPlatform {

class GpReflectObject;

using GpVectorReflectObjWrapBase    = GpVectorWrapperBase<GpReflectObject>;

template<typename T>
using GpVectorReflectObjWrap        = GpVectorWrapper<T, GpReflectObject, std::vector>;

}// namespace GPlatform
