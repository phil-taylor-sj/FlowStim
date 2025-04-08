#include <Core/Solver/PressureExplicit.h>

namespace fstim
{

    std::unique_ptr<vecp::Vec2d[]> PressureExplicit::calculateStructured(
        const Field<double>& pressure, const Mesh& mesh 
    )
    {
        std::unique_ptr<vecp::Vec2d[]> sources = std::make_unique<vecp::Vec2d[]>(mesh.nCells);

        std::unique_ptr<double[]> faceValues = FaceValues<double>::interpolate(pressure, mesh);

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            const Cell& cell = mesh.cells[cellId];

            const Face west = mesh.faces[cell.faceId[int(Compass::WEST)]];
            const Face east = mesh.faces[cell.faceId[int(Compass::EAST)]];
            const Face north = mesh.faces[cell.faceId[int(Compass::NORTH)]];
            const Face south = mesh.faces[cell.faceId[int(Compass::SOUTH)]];

            double xGradient =  (faceValues[east.id] - faceValues[west.id])
                    / (east.center.x - west.center.x);
            double yGradient =  (faceValues[south.id] - faceValues[north.id])
                    / (south.center.y - north.center.y);

            sources[cellId] = vecp::Vec2d(
                cell.volume * xGradient,
                cell.volume * yGradient
            );
        }

        return std::move(sources);
    }
}