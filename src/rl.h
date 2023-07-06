#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-copy"
#pragma clang diagnostic ignored "-Wunused-function"

#include <raylib-cpp.hpp>
namespace rl = raylib;

#pragma clang diagnostic pop

// Allow rl::Vector2 to be used in an unordered_map

template<>
struct std::hash<rl::Vector2> {
    std::size_t operator()(const rl::Vector2& v) const {
        const int& first = reinterpret_cast<const int&>(v.x) << 4;
        const int& second = reinterpret_cast<const int&>(v.y);

        return first | second;
    }
};

template<>
struct std::equal_to<rl::Vector2> {
    bool operator() (rl::Vector2 const& v1, rl::Vector2 const& v2) const {
        return v1.x == v2.x && v1.y == v2.y;
    }
};

