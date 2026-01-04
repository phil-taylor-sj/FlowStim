#include <Core/Solver/ExplicitMethod.h>

#include <iostream>

namespace fstim
{
    template <typename T>
    int ExplicitMethod<T>::operator()(Field<T>& field, const T* source)
    {
        T* values = field.writeValues();
        const T* oldValues = field.readOldValues();
        const SparseMatrix<T>& lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int id = 0; id < field.nCells; id++)
        {
            values[id] = rhs[id];
            // Cycle through the coefficient map for the lhs contributions to the current cell.
            for (const auto coeffId : lhs.getColumnIds(id))
            {
                if (coeffId == id) { continue; } // Skip primary cell coefficient (Ap)
                values[id] -= lhs(id, coeffId) * oldValues[coeffId];
            }
            values[id] /= lhs(id, id);
        }
        return 1;
    }

    template <typename T>
    ExplicitMethod<T>::ExplicitMethod() {};

    template <typename T>
    ExplicitMethod<T>::~ExplicitMethod() {};

    template class ExplicitMethod<double>;

    template class ExplicitMethod<vecp::Vec2d>; 
}