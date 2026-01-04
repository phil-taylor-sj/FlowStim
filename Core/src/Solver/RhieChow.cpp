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
        const SparseMatrix<T>& lhs = field.readLeft();
        const T* rhs = field.readRight();

        std::unique_ptr<T[]> momentum = std::make_unique<T[]>(nCells);

        for (int cellId = 0; cellId < nCells; cellId++)
        {
            T sum = T();
            for (const auto coeffId : lhs.getColumnIds(cellId))
            {
                if (coeffId == cellId) { continue; } // Skip primary cell coefficient (Ap)
                sum -= lhs(cellId, coeffId) * values[coeffId];
            }
            sum += rhs[cellId];
            sum /= lhs(cellId, cellId);
            momentum[cellId] = sum;
        }

        return std::move(momentum);
    }

    template <typename T>
    std::unique_ptr<T[]> RhieChow<T>::interpolatePrimaryCoefficients(Field<T>& field, Mesh2d& mesh)
    {
        const SparseMatrix<T>& lhs = field.readLeft();
        std::unique_ptr<T[]> primaryCellCoefficients = std::make_unique<T[]>(field.nCells);

        for (int cellId = 0; cellId < field.nCells; cellId++)
        {
            primaryCellCoefficients[cellId] = lhs(cellId, cellId);
        }

        std::unique_ptr<T[]> faceValues = FaceValues<T>::interpolate(
            primaryCellCoefficients.get(), mesh
        );

        return std::move(faceValues);
    }

    template class RhieChow<double>;

    template class RhieChow<vecp::Vec2d>;
}