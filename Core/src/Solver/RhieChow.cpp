#include <Core/Solver/RhieChow.h>

namespace fstim
{
    template <typename T>
    std::unique_ptr<T[]> RhieChow<T>::interpolate(Field<T>& field, Mesh& mesh)
    {   
        std::unique_ptr<T[]> faceValues = std::make_unique<T[]>(mesh.nFaces);
        std::unique_ptr<T[]> momentum = RhieChow<T>::sumMomentum(field);

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face& face = mesh.faces[faceId];

            // TODO: Add an owner weight property to each cell to remove need
            // for weight to be recaluclated during simulations.
            if (face.neighId != -1)
            {
                // --- Replace ---
                vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                vecp::Vec2d neighCenter = mesh.cells[face.neighId].center;
                
                double internalWeight = (ownerCenter - neighCenter).mag() / 
                    (ownerCenter - face.center).mag();
                // --------------

                faceValues[faceId] = momentum[face.ownerId] * internalWeight
                    + momentum[face.neighId] * (1. - internalWeight);          

                continue;
            }
            
            faceValues[faceId] = momentum[face.ownerId];
        }

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

    template class RhieChow<double>;

    template class RhieChow<vecp::Vec2d>;
}