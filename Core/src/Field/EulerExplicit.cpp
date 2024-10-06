#include <Core/Field/EulerExplicit.h>

namespace fstim
{
    template <typename T>
    void EulerExplicit<T>::operator()( 
            const Mesh& mesh, 
            Field<T>& field, 
            const double* rho,
            double deltaT)
    {
        const T* values = field.readValues();
        std::map<int, T>* lhs = field.writeLeft();
        T* rhs = field.writeRight();

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            double volume = mesh.cells[cellId].volume;
            double factor = volume * rho[cellId] / deltaT;
            lhs[cellId][cellId] += factor;
            rhs[cellId] += values[cellId] * factor;  
        }
    }

    template class EulerExplicit<double>;

    template class EulerExplicit<vecp::Vec2d>;
}