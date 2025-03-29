#pragma once

#include <VecPlus/Vec2.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/FaceValues.h>

namespace fstim
{
    class PressureExplicit  
    {
    public:
        std::unique_ptr<vecp::Vec2d[]> calculateStructured(
            const Field<double>& pressure, const Mesh& mesh 
        )
        {
            std::unique_ptr<double[]> faceValues = FaceValues<double>::interpolate(pressure, mesh)
        }

    };
}