#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    template <typename T>
    void JacobiMethod<T>::operator()(Field<T>& field, const T* source)
    {
     
        std::unique_ptr<T[]> newValues = std::make_unique<T[]>(field.nCells);

        for (size_t count = 0; count < 10; count++)
        {

            this->m_iteratorLoop(field, source, newValues.get());

            // TODO: call m_checkConvergence.

            T* values = field.writeValues();
            for (int id = 0; id < field.nCells; id++)
            {
                values[id] = newValues[id];
            }
            // TODO: break if convergence criteria reach.
        }
    }

    template <typename T>
    void JacobiMethod<T>::m_iteratorLoop(Field<T>& field, const T* source, T* newValues)
    {
        T* values = field.writeValues();
        const T* oldValues = field.readOldValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int id = 0; id < field.nCells; id++)
        {
            // Set the initial values of the new value.
            newValues[id] = rhs[id];
            if (source != nullptr)
            {
                newValues[id] += source[id];
            }

            // Add all contributions from the left hand side terms.
            for (const std::pair<int, T> pair : lhs[id])
            {
                if (pair.first == id) { continue; }
                newValues[id] -= pair.second * values[pair.first];
            }
            newValues[id] /= lhs[id].at(id);
        }
    }

    template <typename T>
    JacobiMethod<T>::~JacobiMethod() {};

    template class JacobiMethod<double>;

    template class JacobiMethod<vecp::Vec2d>;
}