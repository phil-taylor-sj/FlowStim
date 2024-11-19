#include <Core/Solver/ExplicitMethod.h>

#include <iostream>

namespace fstim
{
    template <typename T>
    void ExplicitMethod<T>::operator()(Field<T>& field, const T* source)
    {
        T* values = field.writeValues();
        const T* oldValues = field.readOldValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int id = 0; id < field.nCells; id++)
        {
            values[id] = rhs[id];
            // Cycle through the coefficient map for the lhs contributions to the current cell.
            for (const std::pair<int, T> pair : lhs[id])
            {
                if (pair.first == id) { continue; } // Skip primary cell coefficient (Ap)
                values[id] -= pair.second * oldValues[pair.first];
            }
            values[id] /= lhs[id].at(id);
        }
    }

    template <typename T>
    ExplicitMethod<T>::ExplicitMethod() {};

    template <typename T>
    ExplicitMethod<T>::~ExplicitMethod() {};

    template class ExplicitMethod<double>;

    template class ExplicitMethod<vecp::Vec2d>; 
}