#pragma once

#include <unordered_dense/unordered_dense.h>

namespace ankerl::unordered_dense {

struct StrViewHash
{
    using is_transparent = void;

    size_t operator()(std::string_view sv) const
    {
        return std::hash<std::string_view>{}(sv);
    }
};

struct StrViewEqual
{
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const
    {
        return lhs == rhs;
    }
};

// unordered_dense_std_string<V>
template<typename V>
using map_std_string = ankerl::unordered_dense::map<std::string, V, StrViewHash, StrViewEqual>;

}// namespace ankerl::unordered_dense
