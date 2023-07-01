#pragma once

#include "rl.h"

typedef rl::Vector2 V2;
typedef rl::Vector3 V3;
typedef rl::Vector4 V4;

#define DEF_TYPE(new, old, size)        \
    static_assert(sizeof(old) == size); \
    typedef old new

DEF_TYPE(f32, float, 4);
DEF_TYPE(f64, double, 8);

DEF_TYPE(i8, char, 1);
DEF_TYPE(i16, short, 2);
DEF_TYPE(i32, int, 4);
DEF_TYPE(i64, long, 8);

DEF_TYPE(u8, unsigned char, 1);
DEF_TYPE(u16, unsigned short, 2);
DEF_TYPE(u32, unsigned int, 4);
DEF_TYPE(u64, unsigned long, 8);
typedef size_t usize;

