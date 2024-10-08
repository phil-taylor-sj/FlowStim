#include <Core/Solver/ExplicitIterator.h>

namespace fstim
{
    template <typename T>
    void ExplicitIterator<T>::operator()(Field<T>& field)
    {
        T* values = field.writeValues();
        const T* oldValues = field.readOldValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int id = 0; id < field.nCells; id++)
        {
            values[id] = rhs[id];
            for (const std::pair<int, T> pair : lhs[id])
            {
                if (pair.first == id) { continue; }
                values[id] -= pair.second * oldValues[pair.first];
            }
            values[id] /= lhs[id][id];
        }
    }
}