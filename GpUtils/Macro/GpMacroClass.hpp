#pragma once

namespace GPlatform {

#define CLASS_REMOVE_CTRS_DEFAULT(NAME) \
            NAME        (void) noexcept = delete;

#define CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete; \
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;


#define CLASS_REMOVE_CTRS_MOVE_COPY(NAME) \
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete; \
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;

#define CLASS_REMOVE_CTRS_MOVE(NAME) \
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_DEFAULT_MOVE(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (NAME&&) noexcept = delete;\
    NAME&   operator=   (NAME&&) noexcept = delete;

#define CLASS_REMOVE_CTRS_COPY(NAME) \
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;

#define CLASS_REMOVE_CTRS_DEFAULT_COPY(NAME) \
            NAME        (void) noexcept = delete;\
            NAME        (const NAME&) noexcept = delete;\
    NAME&   operator=   (const NAME&) noexcept = delete;

#define CLASS_DD(TYPENAME) \
    using this_type = TYPENAME; \
    using C         = GpDeclContainersT<this_type>; \
    using SP        = typename C::SP; \
    using CSP       = typename C::CSP; \
    using WP        = typename C::WP; \
    using CWP       = typename C::CWP;

}// namespace GPlatform
