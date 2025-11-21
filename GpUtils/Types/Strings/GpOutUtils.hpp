#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_STRINGS)

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

#include <string_view>
#include <functional>

namespace GPlatform {

class GP_UTILS_API GpOutUtils
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpOutUtils)

    using OutFnT = std::function<void(std::string_view aStr)>;

public:
                        GpOutUtils  (void) noexcept;
                        ~GpOutUtils (void) noexcept;

    static GpOutUtils&  S           (void) {return sInstance;}

    void                SetOutFns   (OutFnT aOutFn,
                                     OutFnT aErrFn);

    void                Out         (std::string_view aStr) noexcept;
    void                Err         (std::string_view aStr) noexcept;

    void                StdOut      (std::string_view aStr) noexcept;
    void                StdErr      (std::string_view aStr) noexcept;

private:
    OutFnT              iOutFn;
    OutFnT              iErrFn;

    static GpOutUtils   sInstance;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
