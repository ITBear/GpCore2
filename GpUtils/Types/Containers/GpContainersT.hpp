#pragma once

#include "../../GpUtils_global.hpp"

#if defined(GP_USE_CONTAINERS)

#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <tuple>
#include <optional>
#include <string>
#include <string_view>

#include "../Numerics/GpNumericTypes.hpp"
#include "../Pointers/GpSharedPtr.hpp"

namespace GPlatform {

class GpUUID;

template <template<typename...> class C, typename S, typename... Ts>
class GpDeclContainerT
{
public:
    using Val   = C<typename S::value_type, Ts...>;
    using Ptr   = C<typename S::value_type*, Ts...>;
    using CPtr  = C<const typename S::value_type*, Ts...>;
    using Ref   = C<std::reference_wrapper<typename S::value_type>, Ts...>;
    using CRef  = C<std::reference_wrapper<const typename S::value_type>, Ts...>;
    using SP    = C<typename S::SP, Ts...>;
    using CSP   = C<typename S::CSP, Ts...>;
    using WP    = C<typename S::WP, Ts...>;
    using CWP   = C<typename S::CWP, Ts...>;
};

template <template<typename...> class C, typename K, typename S, typename... Ts>
class GpDeclContainerMapT
{
public:
    using Val   = C<K, typename S::value_type, Ts...>;
    using Ptr   = C<K, typename S::value_type*, Ts...>;
    using CPtr  = C<K, const typename S::value_type*, Ts...>;
    using SP    = C<K, typename S::SP, Ts...>;
    using CSP   = C<K, typename S::CSP, Ts...>;
    using WP    = C<K, typename S::WP, Ts...>;
    using CWP   = C<K, typename S::CWP, Ts...>;
};

template <typename T>
class GpDeclContainersT
{
public:
    using value_type    = T;
    using this_type     = GpDeclContainersT<value_type>;

    //Shared pointers
    using SP            = GpSP<T>;
    using CSP           = GpCSP<T>;
    using WP            = GpWP<T>;
    using CWP           = GpCWP<T>;

    //Containers
    using Vec           = GpDeclContainerT<std::vector, this_type>;
    using Set           = GpDeclContainerT<std::set, this_type>;
    using USet          = GpDeclContainerT<std::unordered_set, this_type>;
    using Queue         = GpDeclContainerT<std::queue, this_type>;
    using Deque         = GpDeclContainerT<std::deque, this_type>;
    using Stack         = GpDeclContainerT<std::stack, this_type>;
    using List          = GpDeclContainerT<std::list, this_type>;
    using FList         = GpDeclContainerT<std::forward_list, this_type>;

    using MapStr        = GpDeclContainerMapT<std::map, std::string, this_type, std::less<>>;
    using MapSv         = GpDeclContainerMapT<std::map, std::string_view, this_type, std::less<>>;
    using MapUuid       = GpDeclContainerMapT<std::map, GpUUID, this_type, std::less<>>;
    using MapSi64       = GpDeclContainerMapT<std::map, s_int_64, this_type, std::less<>>;
    using MapUi64       = GpDeclContainerMapT<std::map, u_int_64, this_type, std::less<>>;
    using MapSizeT      = GpDeclContainerMapT<std::map, size_t, this_type, std::less<>>;
    using MapSSizeT     = GpDeclContainerMapT<std::map, ssize_t, this_type, std::less<>>;
    using MapCVoidPtr   = GpDeclContainerMapT<std::map, const void*, this_type, std::less<>>;

    using UMapStr       = GpDeclContainerMapT<std::map, std::string, this_type>;
    using UMapSv        = GpDeclContainerMapT<std::map, std::string_view, this_type>;
    using UMapUuid      = GpDeclContainerMapT<std::unordered_map, GpUUID, this_type>;
    using UMapSi64      = GpDeclContainerMapT<std::unordered_map, s_int_64, this_type>;
    using UMapUi64      = GpDeclContainerMapT<std::unordered_map, u_int_64, this_type>;
    using UMapSizeT     = GpDeclContainerMapT<std::unordered_map, size_t, this_type>;
    using UMapSSizeT    = GpDeclContainerMapT<std::unordered_map, ssize_t, this_type>;
    using UMapCVoidPtr  = GpDeclContainerMapT<std::map, const void*, this_type>;
    using UMapCPtr      = GpDeclContainerMapT<std::map, const this_type*, this_type>;

    using Opt           = GpDeclContainerT<std::optional, this_type>;
    using Ref           = std::reference_wrapper<this_type>;
    using CRef          = std::reference_wrapper<const this_type>;
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
