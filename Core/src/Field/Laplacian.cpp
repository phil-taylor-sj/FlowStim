#include <Core/Field/Laplacian.h>

namespace fstim
{
    template <typename T>
    void Laplacian<T>::operator()(const Mesh& mesh, Field<T>& field, const double* visc)
    {
        const T* values = field.readValues();
        std::map<int, T>* lhs = field.writeLeft();
        T* rhs = field.writeRight();

        for (int cellId = 0; cellId < mesh.nCells; cellId++)
        {
            const Cell& cell = mesh.cells[cellId];
            for (int faceId : cell.faceId)
            {   
                const Face& face = mesh.faces[faceId];
                
                // Retrive length data
                int neighId = (face.ownerId == cellId) 
                    ? face.neighId 
                    : face.ownerId;
                double distance = (neighId != -1)
                    ? (mesh.cells[neighId].center - cell.center).mag()  
                    : (face.center - cell.center).mag();
                double sfMag = face.normal.mag();
                
                // Update and skip to next cycle if face is internal
                if (neighId != -1) 
                {
                    lhs[cellId][cellId] -= -1.0 * (sfMag / distance) * visc[cellId];
                    lhs[cellId][neighId] -=  1.0 * (sfMag / distance) * visc[neighId];
                    continue;
                }

                // If boudnary face:
                // Retrieve boudnary condition and update accordingly
                std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSetId(face.id));              
                switch (std::get<0>(bc)) {
                    case BcType::NONE:
                        [[fallthrough]];
                    case BcType::ZEROGRADIENT:
                        // no change to equation
                        break;
                    case BcType::FIXEDVALUE:
                        // Incorrect: need to take gradient from fixed value
                        // rhs[cellId] += std::get<1>(bc) * (sfMag / distance) * visc[cellId];
                        rhs[cellId] += (std::get<1>(bc) - values[cellId])
                            * (sfMag / distance) * visc[cellId];
                        break;
                    default:
                        break;
                }
            }            
        }
    }

    template <typename T>
    Laplacian<T>::Laplacian() {}

    template class Laplacian<double>;

    template class Laplacian<vecp::Vec2d>;

}