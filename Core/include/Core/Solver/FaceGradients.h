#pragma once

#include <VecPlus/Vec2.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>

namespace fstim
{
    template <typename T>
    class FaceGradients
    {
    public:
        static std::unique_ptr<vecp::Vec2d[]> calculate(const Field<T>& field, const Mesh2d& mesh);

    };
}