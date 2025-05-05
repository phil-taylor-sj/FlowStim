#include <Core/Solver/PressureExplicit.h>

namespace fstim
{

    std::unique_ptr<vecp::Vec2d[]> PressureExplicit::calculateStructured(
        const Field<double>& pressure, const Mesh& mesh 
    )
    {
        std::unique_ptr<vecp::Vec2d[]> sources = std::make_unique<vecp::Vec2d[]>(mesh.nCells);
        std::unique_ptr<vecp::Vec2d[]> faceGradients = FaceGradients<double>::calculate(pressure, mesh);

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            vecp::Vec2d sum {};

            const Cell& cell = mesh.cells[cellId];
            for (int faceId: cell.faceId)
            {
                sum += faceGradients[faceId];
            }
            sources[cellId] = sum / cell.faceId.size();

            /*
            double xGradient =  (faceValues[east.id] - faceValues[west.id])
                    / (east.center.x - west.center.x);
            double yGradient =  (faceValues[south.id] - faceValues[north.id])
                    / (south.center.y - north.center.y);

            sources[cellId] = vecp::Vec2d(
                cell.volume * xGradient,
                cell.volume * yGradient
            );
            */
        }

        return std::move(sources);
    }
}