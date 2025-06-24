#include <Core/Field/PressureLinear.h>

namespace fstim 
{
    template <typename D, typename F>
    void PressureLinear<D, F>::operator()(const Mesh<D, F>& mesh, ScalarField& field, const D* primaryCoeffsAtFaces)
    {
        const double* values = field.readValues();
        std::map<int, double>* lhs = field.writeLeft();
        double* rhs = field.writeRight();

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            const Cell<D, F>& cell = mesh.cells[cellId];
            for (int faceId : cell.faceId)
            {   
                const Face<D>& face = mesh.faces[faceId];
                D surfaceNormal = (cellId == face.ownerId)
                    ? face.normal.normalise()
                    : face.normal.normalise() * -1.;
                
                int neighId = (face.ownerId == cellId) 
                    ? face.neighId
                    : face.ownerId;

                double factor = (surfaceNormal / primaryCoeffsAtFaces[faceId]).dot(
                    cell.volume / face.cellToCellSpacing
                );

                // Update and skip to next cycle if face is internal
                if (neighId != -1) 
                {
                    lhs[cellId][cellId] += -1.0 * factor;
                    lhs[cellId][neighId] += factor;
                    continue;
                }

                // If boudnary face:
                // Retrieve boudnary condition and update accordingly
                std::tuple<BcType, double> bc = field.getBc(mesh.getFaceSetId(face.id));              
                switch (std::get<0>(bc)) {
                    case BcType::NONE:
                        [[fallthrough]];
                    case BcType::ZEROGRADIENT:
                        // no change to equation
                        break;
                    case BcType::FIXEDVALUE:
                        lhs[cellId][cellId] += -1.0 * factor;
                        rhs[cellId] -= factor * std::get<1>(bc);
                        break;
                    default:
                        break;
                }
            }        
        }    

    }

    template class PressureLinear<vecp::Vec2d, vecp::Vec2f>;
    template class PressureLinear<vecp::Vec3d, vecp::Vec3f>;
}