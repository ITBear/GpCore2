#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <array>
#include <string_view>

namespace GPlatform {

enum class GpContainerUpdateStatus
{
    INSERT_NEW,     /**< A new value was set/inserted into the container. */
    KEEP_EXISTING   /**< An existing value in the container was used. */
};

}// namespace GPlatform

// ------------------------------- std::to_string -----------------------------------

namespace std {

inline const ::std::string_view to_string(::GPlatform::GpContainerUpdateStatus aStatus)
{
    static constexpr ::std::array<std::string_view, 2> sToString =
    {
        ::std::string_view{"INSERT_NEW"},
        ::std::string_view{"KEEP_EXISTING"}
    };

    return sToString.at(size_t(aStatus));
}

}// namespace std

#endif// #if defined(GP_USE_CONTAINERS)
