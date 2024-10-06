#pragma once

#include <Core/Field/Field.h>
#include <Core/Field/Divergence.h>
#include <Core/Field/Laplacian.h>
#include <Core/Field/EulerExplicit.h>

namespace fstim
{
    template<typename T>
    class FieldEquation : public Field<T>
    {

    public:
        std::unique_ptr<Divergence<T>> divergence = std::make_unique<Divergence<T>>();
        std::unique_ptr<Laplacian<T>> laplacian = std::make_unique<Laplacian<T>>();
        std::unique_ptr<EulerExplicit<T>> temporal = std::make_unique<EulerExplicit<T>>();

        FieldEquation(int nCells) : Field<T>(nCells) { }

        ~FieldEquation() = default;

        FieldEquation(const FieldEquation& newField) = delete;

        FieldEquation& operator=(const FieldEquation& newField) = delete;

    };

    using ScalarField = Field<double>;

    using VectorField = Field<vecp::Vec2d>;
}