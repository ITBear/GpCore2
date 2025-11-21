#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/Types/Pointers/GpSharedPtr.hpp>
#include <GpCore2/GpUtils/Types/Pointers/GpWeakPtr.hpp>
#include <GpCore2/GpUtils/Types/Pointers/GpSpan.hpp>

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
#include <optional>
#include <string_view>
#include <functional>
#include <memory>

namespace GPlatform {

class GpUUID;

template <template<typename...> class C, typename S, typename... Ts>
class GpDeclContainerT
{
public:
    using Val   = C<typename S::value_type, Ts...>;
    using CVal  = C<const typename S::value_type, Ts...>;
    using Ptr   = C<typename S::value_type*, Ts...>;
    using CPtr  = C<const typename S::value_type*, Ts...>;
    using Ref   = C<std::reference_wrapper<typename S::value_type>, Ts...>;
    using CRef  = C<std::reference_wrapper<const typename S::value_type>, Ts...>;
    using SP    = C<typename S::SP, Ts...>;
    using CSP   = C<typename S::CSP, Ts...>;
    using UP    = C<typename S::UP, Ts...>;
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
    using UP    = C<K, typename S::UP, Ts...>;
    using WP    = C<K, typename S::WP, Ts...>;
    using CWP   = C<K, typename S::CWP, Ts...>;
};

template <typename S, std::size_t N>
class GpDeclSmallVecT
{
public:
    using Val   = boost::container::small_vector<typename S::value_type, N>;
    using CVal  = boost::container::small_vector<const typename S::value_type, N>;
    using Ptr   = boost::container::small_vector<typename S::value_type*, N>;
    using CPtr  = boost::container::small_vector<const typename S::value_type*, N>;
    using Ref   = boost::container::small_vector<std::reference_wrapper<typename S::value_type>, N>;
    using CRef  = boost::container::small_vector<std::reference_wrapper<const typename S::value_type>, N>;
    using SP    = boost::container::small_vector<typename S::SP, N>;
    using CSP   = boost::container::small_vector<typename S::CSP, N>;
    using UP    = boost::container::small_vector<typename S::UP, N>;
    using WP    = boost::container::small_vector<typename S::WP, N>;
    using CWP   = boost::container::small_vector<typename S::CWP, N>;
};

template <typename K, typename S, std::size_t N>
class GpDeclContainerSmallFlatMapT
{
public:
    using Val   = boost::container::small_flat_map<K, typename S::value_type, N, std::less<>>;
    using Ptr   = boost::container::small_flat_map<K, typename S::value_type*, N, std::less<>>;
    using CPtr  = boost::container::small_flat_map<K, const typename S::value_type*, N, std::less<>>;
    using SP    = boost::container::small_flat_map<K, typename S::SP, N, std::less<>>;
    using CSP   = boost::container::small_flat_map<K, typename S::CSP, N, std::less<>>;
    using UP    = boost::container::small_flat_map<K, typename S::UP, N, std::less<>>;
    using WP    = boost::container::small_flat_map<K, typename S::WP, N, std::less<>>;
    using CWP   = boost::container::small_flat_map<K, typename S::CWP, N, std::less<>>;
};

template <typename K, typename S>
class GpDeclContainerFlatMapT
{
public:
    using Val   = boost::container::flat_map<K, typename S::value_type, std::less<>>;
    using Ptr   = boost::container::flat_map<K, typename S::value_type*, std::less<>>;
    using CPtr  = boost::container::flat_map<K, const typename S::value_type*, std::less<>>;
    using SP    = boost::container::flat_map<K, typename S::SP, std::less<>>;
    using CSP   = boost::container::flat_map<K, typename S::CSP, std::less<>>;
    using UP    = boost::container::flat_map<K, typename S::UP, std::less<>>;
    using WP    = boost::container::flat_map<K, typename S::WP, std::less<>>;
    using CWP   = boost::container::flat_map<K, typename S::CWP, std::less<>>;
};

template <typename T>
class GpDeclContainersT
{
public:
    using value_type    = T;
    using this_type     = GpDeclContainersT<value_type>;

    // Shared pointers
    using SP            = GpSP<T>;
    using CSP           = GpCSP<T>;
    using UP            = std::unique_ptr<T>;
    using WP            = GpWP<T>;
    using CWP           = GpCWP<T>;

    // Containers
    using Vec           = GpDeclContainerT<std::vector, this_type>;
    using Span          = GpDeclContainerT<GpSpan, this_type>;
    using Set           = GpDeclContainerT<std::set, this_type>;
    using USet          = GpDeclContainerT<std::unordered_set, this_type>;
    using Queue         = GpDeclContainerT<std::queue, this_type>;
    using Deque         = GpDeclContainerT<std::deque, this_type>;
    using Stack         = GpDeclContainerT<std::stack, this_type>;
    using List          = GpDeclContainerT<std::list, this_type>;
    using FList         = GpDeclContainerT<std::forward_list, this_type>;
    template<std::size_t N>
    using SmallVec      = GpDeclSmallVecT<this_type, N>;
    using Opt           = std::optional<T>;
    using Ref           = std::reference_wrapper<T>;

    using MapStr        = GpDeclContainerMapT<std::map, std::string, this_type, std::less<>>;
    using MapSv         = GpDeclContainerMapT<std::map, std::string_view, this_type, std::less<>>;
    using MapUuid       = GpDeclContainerMapT<std::map, GpUUID, this_type, std::less<>>;
    using MapSi64       = GpDeclContainerMapT<std::map, s_int_64, this_type, std::less<>>;
    using MapUi64       = GpDeclContainerMapT<std::map, u_int_64, this_type, std::less<>>;
    using MapSi32       = GpDeclContainerMapT<std::map, s_int_32, this_type, std::less<>>;
    using MapUi32       = GpDeclContainerMapT<std::map, u_int_32, this_type, std::less<>>;
    using MapSizeT      = GpDeclContainerMapT<std::map, size_t, this_type, std::less<>>;
    using MapSSizeT     = GpDeclContainerMapT<std::map, ssize_t, this_type, std::less<>>;
    using MapCVoidPtr   = GpDeclContainerMapT<std::map, const void*, this_type, std::less<>>;

    template<std::size_t N>
    using SmallFlatMapStr       = GpDeclContainerSmallFlatMapT<std::string, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapSv        = GpDeclContainerSmallFlatMapT<std::string_view, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapUuid      = GpDeclContainerSmallFlatMapT<GpUUID, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapSi64      = GpDeclContainerSmallFlatMapT<s_int_64, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapUi64      = GpDeclContainerSmallFlatMapT<u_int_64, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapSi32      = GpDeclContainerSmallFlatMapT<s_int_32, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapUi32      = GpDeclContainerSmallFlatMapT<u_int_32, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapSizeT     = GpDeclContainerSmallFlatMapT<size_t, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapSSizeT    = GpDeclContainerSmallFlatMapT<ssize_t, this_type, N>;
    template<std::size_t N>
    using SmallFlatMapCVoidPtr  = GpDeclContainerSmallFlatMapT<const void*, this_type, N>;

    using FlatMapStr        = GpDeclContainerFlatMapT<std::string, this_type>;
    using FlatMapSv         = GpDeclContainerFlatMapT<std::string_view, this_type>;
    using FlatMapUuid       = GpDeclContainerFlatMapT<GpUUID, this_type>;
    using FlatMapSi64       = GpDeclContainerFlatMapT<s_int_64, this_type>;
    using FlatMapUi64       = GpDeclContainerFlatMapT<u_int_64, this_type>;
    using FlatMapSi32       = GpDeclContainerFlatMapT<s_int_32, this_type>;
    using FlatMapUi32       = GpDeclContainerFlatMapT<u_int_32, this_type>;
    using FlatMapSizeT      = GpDeclContainerFlatMapT<size_t, this_type>;
    using FlatMapSSizeT     = GpDeclContainerFlatMapT<ssize_t, this_type>;
    using FlatMapCVoidPtr   = GpDeclContainerFlatMapT<const void*, this_type>;

    using UMapStr       = GpDeclContainerMapT<std::map, std::string, this_type>;
    using UMapSv        = GpDeclContainerMapT<std::map, std::string_view, this_type>;
    using UMapUuid      = GpDeclContainerMapT<std::unordered_map, GpUUID, this_type>;
    using UMapSi64      = GpDeclContainerMapT<std::unordered_map, s_int_64, this_type>;
    using UMapUi64      = GpDeclContainerMapT<std::unordered_map, u_int_64, this_type>;
    using UMapSizeT     = GpDeclContainerMapT<std::unordered_map, size_t, this_type>;
    using UMapSSizeT    = GpDeclContainerMapT<std::unordered_map, ssize_t, this_type>;
    using UMapCVoidPtr  = GpDeclContainerMapT<std::map, const void*, this_type>;
    using UMapCPtr      = GpDeclContainerMapT<std::map, const this_type*, this_type>;

    using Opts          = GpDeclContainerT<std::optional, this_type>;
    using Refs          = GpDeclContainerT<std::reference_wrapper, this_type>;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
