#include <Core/Solver/Courant.h>
       
namespace fstim
{        
    double Courant::calculateMax(
            double timeStep,
            const Mesh& mesh,
            const VectorField& velocity)
    {
        const vecp::Vec2d* values = velocity.readValues();
        
        double maxCFL = 0.;
        for (int id = 0; id < mesh.nCells; id++)
        {
            double lengthScale = std::sqrt(mesh.cells[id].volume);
            double cfl = abs(values[id].y);// * timeStep / lengthScale;
            maxCFL = std::max(maxCFL, cfl);
        }
        return maxCFL;
    }

    double Courant::calculateAve(
            double timeStep,
            const Mesh& mesh,
            const VectorField& velocity)
    {
        const vecp::Vec2d* values = velocity.readValues();
        
        double aveCFL = 0.;
        for (int id = 0; id < mesh.nCells; id++)
        {
            double lengthScale = std::sqrt(mesh.cells[id].volume);
            double cfl = values[id].mag();// * timeStep / lengthScale;
            aveCFL += cfl / mesh.nCells;
        }

        return aveCFL;
    }
}