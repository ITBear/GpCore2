#pragma once

#include "GpUtils_global.hpp"

/*******************************************************************************
 *                               WARNINGS DISABLE
 ******************************************************************************/

#define DIAG_STR(s) #s
#define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)
#ifdef _MSC_VER
#   define DIAG_DO_PRAGMA(x) __pragma (#x)
#   define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(warning(x))
#else
#   define DIAG_DO_PRAGMA(x) _Pragma (#x)
#   define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)
#endif

#define GP_WARNING_PUSH()           DIAG_PRAGMA(GP_COMPILER_NAME, push)
#define GP_WARNING_POP()            DIAG_PRAGMA(GP_COMPILER_NAME, pop)
#define GP_WARNING_DISABLE(option)  DIAG_PRAGMA(GP_COMPILER_NAME, ignored DIAG_JOINSTR(-W,option))

GP_WARNING_DISABLE(pragmas);

/*******************************************************************************
 *                               CLASS DEFINITION
 ******************************************************************************/

#include <experimental/type_traits>

namespace GPlatform {

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
    using SP        = C::SP; \
    using CSP       = C::CSP; \
    using WP        = C::WP; \
    using CWP       = C::CWP;

#define CLASS_TAG(TAG_NAME) static constexpr bool __TAG_##TAG_NAME##__ (void) noexcept {return true;}

//TODO: remove std::experimental::is_detected

#define CLASS_TAG_DETECTOR(TAG_NAME) \
    template<typename TAGGED_TYPE> \
    using STAG_FN_DETECTOR__##TAG_NAME = decltype(std::declval<TAGGED_TYPE&>().__TAG_##TAG_NAME##__()); \
\
    template<typename TAGGED_TYPE> \
    static constexpr bool SHasTag_##TAG_NAME (void) {return std::experimental::is_detected_v<STAG_FN_DETECTOR__##TAG_NAME, TAGGED_TYPE>;}

}//namespace GPlatform
