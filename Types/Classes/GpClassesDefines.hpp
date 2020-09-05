#pragma once

#include "../../Config/GpConfig.hpp"

//TODO: remove std::experimental::is_detected
#include <experimental/type_traits>

namespace GPlatform {

#define CLASS_REMOVE_CTRS(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (const NAME&) noexcept = delete;\
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(NAME) \
            NAME        (const NAME&) noexcept = delete;\
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_EXCEPT_DEFAULT_COPY(NAME) \
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_EXCEPT_COPY(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_EXCEPT_DEFAULT_MOVE(NAME) \
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;

#define CLASS_REMOVE_CTRS_EXCEPT_MOVE(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;

#define CLASS_DECLARE_DEFAULTS(TYPENAME) \
    using this_type = TYPENAME; \
    using C         = GpDeclContainersT<this_type>; \
    using SP        = C::SP; \
    using CSP       = C::CSP; \
    using WP        = C::WP; \
    using CWP       = C::CWP;

#define CLASS_TAG(TAG_NAME) static constexpr bool __TAG_##TAG_NAME##__ (void) noexcept {return true;}

#define CLASS_TAG_DETECTOR(TAG_NAME) \
    template<typename TAGGED_TYPE> \
    using STAG_FN_DETECTOR__##TAG_NAME = decltype(std::declval<TAGGED_TYPE&>().__TAG_##TAG_NAME##__()); \
\
    template<typename TAGGED_TYPE> \
    static consteval bool SHasTag_##TAG_NAME (void) {return std::experimental::is_detected_v<STAG_FN_DETECTOR__##TAG_NAME, TAGGED_TYPE>;}

//TODO: remove std::experimental::is_detected

}//GPlatform
