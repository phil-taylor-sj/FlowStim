#include <Core/Field/Laplacian.h>

namespace fstim
{
    void Laplacian::operator(const Mesh& mesh, FieldEquation<T>& field, const double* mu[]){

        const T* values[] = field.readValues();
        std::map<T>* lhs[] = field.writeLeft();
        T* rhs[] = field.writeRight();

        for (int cellId = 0; ownerId < mesh.nCells; cellId++)
        {
            Cell& cell = mesh.cells[cellId];
            for (int faceId : cell.faceIds)
            {   
                Face& face = mesh.faces[faceId];
                
                // Retrive length data
                int neighId = (face.ownerId == cellId) 
                    ? face.neighId 
                    : face.ownerId;
                double distance = (neighId != -1)
                    ? (mesh.cells[neighId].center - cell.center).mag()  
                    : (face.center - cell.center).mag();
                double sfMag = face.normal.mag();
                
                // Update and skip to next cycle if face is internal
                if (neighId != 0) 
                {
                    lhs[cellId][cellId] -= -1.0 * (sfMag / distance) * mu[cellId];
                    lhs[cellId][neighId] -=  1.0 * (sfMag / distance) * mu[neighId];
                    continue;
                }

                // If boudnary face:
                // Retrieve boudnary condition and update accordingly
                mesh.getFaceSet(face.id);
                std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSet(face.id));              
                switch (std::get<0>(bc)) {
                    case BcType::ZEROGRADIENT:
                        // no change to equation
                        break;
                    case BcType::FIXEDVALUE:
                        rhs[cellId] += std::get<1>(bc) * (sfMag / distance) * mu[cellId];
                        break;
                    default:
                        break;
                }

            }            
        }

    }

    Laplacian::Laplacian() {}

}