#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

namespace fstim
{
    template <typename D, typename F>
    class PressureLinear
    {
    public:
        void operator()(const Mesh<D, F>& mesh, ScalarField& field, const D* primaryCoeffsAtFaces);

        PressureLinear() {};

        virtual ~PressureLinear() = default;
    };

    extern template class PressureLinear<vecp::Vec2d, vecp::Vec2f>;
    extern template class PressureLinear<vecp::Vec3d, vecp::Vec2f>;

    using PressureLinear2d = PressureLinear<vecp::Vec2d, vecp::Vec2f>;
    using PressureLinear3d = PressureLinear<vecp::Vec3d, vecp::Vec3f>;

}