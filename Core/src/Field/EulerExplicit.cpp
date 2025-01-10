#include <Core/Field/EulerExplicit.h>

namespace fstim
{
    template <typename T>
    void EulerExplicit<T>::operator()( 
            double deltaT,
            const Mesh& mesh, 
            Field<T>& field, 
            const double* rho
            )
    {
        const T* values = field.readValues();
        std::map<int, T>* lhs = field.writeLeft();
        T* rhs = field.writeRight();

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            double factor = mesh.cells[cellId].volume / deltaT;
            if (rho != nullptr) { factor *= rho[cellId]; }

            //lhs[cellId][cellId] *= factor;
            rhs[cellId] += values[cellId] * factor;  
            rhs[cellId] -= values[cellId] * lhs[cellId][cellId];
            lhs[cellId][cellId] = T() + factor;
        }
    }

    template <typename T>
    EulerExplicit<T>::EulerExplicit() {}

    template class EulerExplicit<double>;

    template class EulerExplicit<vecp::Vec2d>;
}