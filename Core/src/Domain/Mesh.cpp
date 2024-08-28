#include <Core/Domain/Mesh.h>

namespace fstim
{
    Mesh::Mesh(int nCellsIn, int nFacesIn,  std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn) 
        : nCells(nCellsIn), nFaces(nFacesIn), cells(std::move(cellsIn)), faces(std::move(facesIn))
    {
        
    }
}