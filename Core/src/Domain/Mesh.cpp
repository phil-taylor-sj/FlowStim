#include <Core/Domain/Mesh.h>

namespace fstim
{
    Mesh::Mesh(int nCellsIn, int nFacesIn,  std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn, vecp::Vec2f lengthIn) 
        : nCells(nCellsIn), nFaces(nFacesIn), cells(std::move(cellsIn)), faces(std::move(facesIn)), length(lengthIn)
    {
        
    }
}