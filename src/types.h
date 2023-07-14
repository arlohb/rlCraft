#pragma once

#include "rl.h"

using V2 = rl::Vector2;
using V3 = rl::Vector3;
using V4 = rl::Vector4;

#define DEF_TYPE(new, old, size)        \
    static_assert(sizeof(old) == size); \
    using new = old

DEF_TYPE(f32, float, 4);
DEF_TYPE(f64, double, 8);

DEF_TYPE(i8, char, 1);
DEF_TYPE(i16, short, 2);
DEF_TYPE(i32, int, 4);
#ifdef _WIN32
    DEF_TYPE(i64, long long, 8);
#else
    DEF_TYPE(i64, long, 8);
#endif

DEF_TYPE(u8, unsigned char, 1);
DEF_TYPE(u16, unsigned short, 2);
DEF_TYPE(u32, unsigned int, 4);
#ifdef _WIN32
    DEF_TYPE(u64, unsigned long long, 8);
#else
    DEF_TYPE(u64, unsigned long, 8);
#endif
using usize = size_t;

