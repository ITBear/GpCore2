#pragma once

namespace GPlatform {

#define TAG_SET(TAG_NAME) static constexpr bool __TAG_##TAG_NAME##__ (void) noexcept {return true;}

#define TAG_REGISTER(TAG_NAME) \
template <typename T, typename = void> \
struct __gp_has_tag__##TAG_NAME##__ : std::false_type {}; \
\
template<typename T> \
struct __gp_has_tag__##TAG_NAME##__<T, std::void_t<decltype(std::declval<T>().__TAG_##TAG_NAME##__())>> : std::true_type {}; \
\
template<typename T> \
constexpr bool GpHasTag_##TAG_NAME (void) \
{ \
    return __gp_has_tag__##TAG_NAME##__<T>::value; \
}

}// namespace GPlatform
