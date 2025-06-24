#pragma once

#include <VecPlus/Vec2.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/FaceGradients.h>
#include <Core/Domain/Compass.h>

namespace fstim
{
    /**
     * \brief Calculates explicit pressure source terms for a structured mesh.
     *
     * This function computes pressure source terms for each cell in a structured mesh by:
     * - Interpolating the cell-centered pressure field to the faces using a dedicated interpolation method.
     * - Computing the pressure gradients in the x-direction (using east and west face values) and 
     *   the y-direction (using north and south face values) via finite differences.
     * - Scaling the computed gradients by the cell volume to obtain the source terms.
     *
     * The resulting source term for each cell is represented as a 2D vector, with its components
     * corresponding to the x and y contributions.
     *
     * \param pressure The field containing the cell-centered pressure values.
     * \param mesh The mesh data structure that provides connectivity and geometric information for cells and faces.
     * \return A unique pointer to an array of 2D vectors, where each vector represents the pressure source term
     *         for a corresponding cell in the mesh.
     */

    class PressureExplicit  
    {
    public:
        static std::unique_ptr<vecp::Vec2d[]> calculateStructured(
            const Field<double>& pressure, const Mesh2d& mesh 
        );


    };
}