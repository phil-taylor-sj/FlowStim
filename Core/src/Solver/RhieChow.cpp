#include <Core/Solver/RhieChow.h>

namespace fstim
{
    template <typename T>
    std::unique_ptr<T[]> RhieChow<T>::interpolate(Field<T>& field, Mesh2d& mesh)
    {   
        std::unique_ptr<T[]> momentum = RhieChow<T>::sumMomentum(field);

        std::unique_ptr<T[]> faceValues = FaceValues<T>::interpolate(momentum.get(), mesh);

        return std::move(faceValues);
    }

    template <typename T>
    std::unique_ptr<T[]> RhieChow<T>::sumMomentum(Field<T>& field)
    {
        int nCells = field.nCells;
        const T* values = field.readValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        std::unique_ptr<T[]> momentum = std::make_unique<T[]>(nCells);

        for (int cellId = 0; cellId < nCells; cellId++)
        {
            T sum = T();
            for (const std::pair<int, T> pair : lhs[cellId])
            {
                if (pair.first == cellId) { continue; } // Skip primary cell coefficient (Ap)
                sum -= pair.second * values[pair.first];
            }
            sum += rhs[cellId];
            sum /= lhs[cellId].at(cellId);
            momentum[cellId] = sum;
        }

        return std::move(momentum);
    }

    template <typename T>
    std::unique_ptr<T[]> RhieChow<T>::interpolatePrimaryCoefficients(Field<T>& field, Mesh2d& mesh)
    {
        const std::map<int, T>* lhs = field.readLeft();
        std::unique_ptr<T[]> primaryCellCoefficients = std::make_unique<T[]>(field.nCells);

        for (int cellId = 0; cellId < field.nCells; cellId++)
        {
            primaryCellCoefficients[cellId] = lhs[cellId].at(cellId);
        }

        std::unique_ptr<T[]> faceValues = FaceValues<T>::interpolate(
            primaryCellCoefficients.get(), mesh
        );

        return std::move(faceValues);
    }

    template class RhieChow<double>;

    template class RhieChow<vecp::Vec2d>;
}