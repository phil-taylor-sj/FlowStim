#include <Core/Field/Divergence.h>

namespace fstim
{
    template <typename T>
    void Divergence<T>::operator()(
            const Mesh& mesh, 
            Field<T>& field, 
            const VectorField& velocity,
            const double* rho)
    {

        const T* values = field.readValues();
        std::map<int, T>* lhs = field.writeLeft();
        T* rhs = field.writeRight();
        //const vecp::Vec2d* fluxes[] = velocity.readValues();

        std::unique_ptr<double[]> fluxes = FaceFlux::Linear(mesh, velocity, rho);

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            const Cell& cell = mesh.cells[cellId];
            for (int faceId : cell.faceId)
            {   
                const Face& face = mesh.faces[faceId];
                
                double flux = (face.ownerId == cellId) 
                    ? fluxes[faceId]
                    : -1. * fluxes[faceId];

                int neighId = (face.ownerId == cellId) 
                    ? face.neighId 
                    : face.ownerId;
                
                // Update and skip to next cycle if face is internal
                if (neighId != -1) 
                {                
                    // Retrive length data
                    double distance = (mesh.cells[neighId].center - cell.center).mag();
                    double internalWeight = (cell.center - face.center).mag() / distance;
                    lhs[cellId][cellId] += flux * internalWeight;
                    lhs[cellId][neighId] +=  flux * (1. - internalWeight);
                    continue;
                }

                // If boudnary face:
                // Retrieve boudnary condition and update accordingly
                std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSetId(face.id));              
                switch (std::get<0>(bc)) {
                    case BcType::ZEROGRADIENT:
                        lhs[cellId][cellId] += flux;
                        break;
                    case BcType::FIXEDVALUE:
                        rhs[cellId] -= std::get<1>(bc) * flux;
                        break;
                    default:
                        break;
                }
            }            
        }
    }

    template <typename T>
    Divergence<T>::Divergence() {}

    template class Divergence<double>;

    template class Divergence<vecp::Vec2d>;
}