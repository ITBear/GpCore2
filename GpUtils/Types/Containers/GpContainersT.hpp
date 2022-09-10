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

#define GpArray         std::array
#define GpVector        std::vector
#define GpDeque         std::deque
#define GpList          std::list
#define GpForwardList   std::forward_list
#define GpStack         std::stack
#define GpQueue         std::queue
#define GpPriorityQueue std::priority_queue
#define GpMap           std::map
#define GpMultiMap      std::multimap
#define GpUMap          std::unordered_map
#define GpUMultiMap     std::unordered_multimap
#define GpSet           std::set
#define GpMultiSet      std::multiset
#define GpUSet          std::unordered_set
#define GpUMultiSet     std::unordered_multiset
#define GpTuple         std::tuple
#define GpOptional      std::optional

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
    using Vec           = GpDeclContainerT<GpVector, this_type>;
    using Set           = GpDeclContainerT<GpSet, this_type>;
    using USet          = GpDeclContainerT<GpUSet, this_type>;
    using Queue         = GpDeclContainerT<GpQueue, this_type>;
    using Deque         = GpDeclContainerT<GpDeque, this_type>;
    using Stack         = GpDeclContainerT<GpStack, this_type>;
    using List          = GpDeclContainerT<GpList, this_type>;
    using FList         = GpDeclContainerT<GpForwardList, this_type>;

    using MapStr        = GpDeclContainerMapT<GpMap, std::string, this_type, std::less<>>;
    using MapSv         = GpDeclContainerMapT<GpMap, std::string_view, this_type, std::less<>>;
    using MapUuid       = GpDeclContainerMapT<GpMap, GpUUID, this_type, std::less<>>;
    using MapSi64       = GpDeclContainerMapT<GpMap, s_int_64, this_type, std::less<>>;
    using MapUi64       = GpDeclContainerMapT<GpMap, u_int_64, this_type, std::less<>>;
    using MapSizeT      = GpDeclContainerMapT<GpMap, size_t, this_type, std::less<>>;
    using MapSSizeT     = GpDeclContainerMapT<GpMap, ssize_t, this_type, std::less<>>;
    using MapCVoidPtr   = GpDeclContainerMapT<GpMap, const void*, this_type, std::less<>>;

    using UMapStr       = GpDeclContainerMapT<GpMap, std::string, this_type>;
    using UMapSv        = GpDeclContainerMapT<GpMap, std::string_view, this_type>;
    using UMapUuid      = GpDeclContainerMapT<GpUMap, GpUUID, this_type>;
    using UMapSi64      = GpDeclContainerMapT<GpUMap, s_int_64, this_type>;
    using UMapUi64      = GpDeclContainerMapT<GpUMap, u_int_64, this_type>;
    using UMapSizeT     = GpDeclContainerMapT<GpUMap, size_t, this_type>;
    using UMapSSizeT    = GpDeclContainerMapT<GpUMap, ssize_t, this_type>;
    using UMapCVoidPtr  = GpDeclContainerMapT<GpMap, const void*, this_type>;
    using UMapCPtr      = GpDeclContainerMapT<GpMap, const this_type*, this_type>;

    using Opt           = GpDeclContainerT<GpOptional, this_type>;
    using Ref           = std::reference_wrapper<this_type>;
    using CRef          = std::reference_wrapper<const this_type>;
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
