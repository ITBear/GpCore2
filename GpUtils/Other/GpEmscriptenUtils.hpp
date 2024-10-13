#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(__EMSCRIPTEN__)

#include <emscripten.h>
#include <emscripten/bind.h>

namespace GPlatform {

emscripten::val WasmEmscriptenExceptionCatcherVoid  (std::function<void()> aFn);
emscripten::val WasmEmscriptenExceptionCatcher      (std::function<emscripten::val()> aFn);
emscripten::val WasmEmscriptenJsonApiError          (std::string_view aMessage);

}// namespace GPlatform

#endif// #if defined(__EMSCRIPTEN__)
