#include <GpCore2/GpUtils/Other/GpEmscriptenUtils.hpp>

#if defined(__EMSCRIPTEN__)

#include <GpCore2/GpUtils/Types/Strings/GpStringLiterals.hpp>

namespace GPlatform {

emscripten::val WasmEmscriptenExceptionCatcherVoid (std::function<void()> aFn)
{
    emscripten::val resVal = emscripten::val::object();

    try
    {
        aFn();
        resVal.set("data", emscripten::val::undefined());
    } catch (const std::exception& e)
    {
        resVal = WasmEmscriptenJsonApiError(e.what());
    } catch (...)
    {
        resVal = WasmEmscriptenJsonApiError("Unknown exception"_sv);
    }

    return resVal;
}

emscripten::val WasmEmscriptenExceptionCatcher (std::function<emscripten::val()> aFn)
{
    emscripten::val resVal = emscripten::val::object();

    try
    {
        resVal.set("data", aFn());
    } catch (const std::exception& e)
    {
        resVal = WasmEmscriptenJsonApiError(e.what());
    } catch (...)
    {
        resVal = WasmEmscriptenJsonApiError("Unknown exception"_sv);
    }

    return resVal;
}

emscripten::val WasmEmscriptenJsonApiError (std::string_view aMessage)
{
    emscripten::val error = emscripten::val::object();

    error.set("error", std::string(aMessage));

    return error;
}

}// namespace GPlatform

#endif// #if defined(__EMSCRIPTEN__)
