#pragma once

#include <VecPlus/Vec2.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/FaceValues.h>

namespace fstim
{
    template <typename T>
    class RhieChow
    {
    public:

        /**
         * \brief Computes interpolated face velocities using Rhie-Chow interpolation.
         *
         * After the momentum predictor step, the velocity field is known at cell centers.
         * To update the pressure field, these cell-centered velocities must be accurately interpolated
         * to the mesh faces. Direct interpolation can lead to non-physical oscillations (checkerboarding)
         * in the pressure field. This function applies the Rhie-Chow correction, incorporating the
         * pressure gradient to prevent decoupling between pressure and velocity.
         *
         * \param field A reference to the cell-centered velocity field.
         * \param mesh The mesh data structure providing face connectivity and geometric information.
         * \return A unique pointer to an array containing the interpolated face velocities.
         *         The array size corresponds to the number of faces in the mesh (nFaces).
         */
        static std::unique_ptr<T[]> interpolate(Field<T>& field, Mesh2d& mesh);

        /**
         * \brief Calculates the momentum predictor values at cell centers.
         *
         * This function computes the intermediate momentum predictor for each cell, which is essential
         * for the Rhie-Chow interpolation. The momentum predictor is defined as:
         *
         * \f[
         * u_{\text{pred}} = \frac{\sum (A_{ij} \cdot u_j) + b_i}{A_i}
         * \f]
         *
         * where:
         * - \(A_i\) is the diagonal coefficient for the cell,
         * - \(A_{ij}\) represents the coefficients associated with neighboring cells,
         * - \(u_j\) are the velocities of the neighboring cells,
         * - \(b_i\) is the source term.
         *
         * These computed values serve as the basis for constructing the face velocities, ensuring
         * a consistent coupling between the momentum and pressure fields.
         *
         * \param field A reference to the cell-centered velocity field.
         * \return A unique pointer to an array containing the momentum predictor values for each cell.
         *         The array size matches the number of cells in the mesh (nCells).
         */
        static std::unique_ptr<T[]> sumMomentum(Field<T>& field);

        static std::unique_ptr<T[]> interpolatePrimaryCoefficients(Field<T>& field, Mesh2d& mesh);

    };
}