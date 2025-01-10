#pragma once

#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>

#include <VecPlus/Vec2.h>

namespace fstim
{
    template<typename T>
    class Initialise
    {
        static void rectangle(const Mesh& mesh, Field<T>& field, vecp::Vec2i center, vecp::Vec2i widths);
    };
}