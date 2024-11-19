#pragma once

#include <VecPlus/Vec2.h>

namespace fstim
{
    template <typename T>
    class Tolerance
    {
    public:
        T absolute{};
        T relative{};

        Tolerance(T newAbs, T newRel) : absolute(newAbs), relative(newRel) {};
    };
}