#pragma once

#include <VecPlus/Vec2.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>

namespace fstim
{
    template <typename T>
    class FaceValues
    {
    public:

    /**
     * \brief Interpolates face values from cell-centered field data using a Rhie-Chow scheme.
     *
     * This function calculates the face values necessary for solving the flow equations by 
     * interpolating from the cell-centered values. For internal faces (i.e. faces with a valid 
     * neighboring cell), it computes a weighted average of the momentum values from the owner 
     * and neighbor cells. The weight is based on the distance between the cell centers and the 
     * face center, ensuring a smooth transition that minimises non-physical oscillations.
     *
     * For boundary faces (faces with no valid neighbor), the function applies the appropriate 
     * boundary condition as specified in the field. Depending on the type of boundary condition:
     * - \c ZEROGRADIENT: The face value is taken directly from the owner cell value.
     * - \c FIXEDVALUE: A fixed face value is applied.
     *
     * \tparam T The data type used for the field and mesh values.
     * \param field The field object that provides the cell-centered values and boundary condition data.
     * \param mesh The mesh object containing face connectivity and geometric information.
     * \return A unique pointer to an array of interpolated face values. The array size matches the 
     *         total number of faces in the mesh.
    */
        static std::unique_ptr<T[]> interpolate(const Field<T>& field, const Mesh& mesh);

        static std::unique_ptr<T[]> interpolate(const T* cellValues, const Mesh& mesh);
    };
}