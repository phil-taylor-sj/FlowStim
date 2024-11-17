#pragma once

#include <VecPlus/Vec2.h>

namespace fstim
{
    template <typename T>
    struct Tolerance
    {
    public:
        T absolute{};
        T relative{};
    };
}