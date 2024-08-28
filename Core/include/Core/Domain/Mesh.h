#pragma once
#include <vector>

#include <VecPlus/Vec2.h>
#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>

namespace fstim
{
    class Mesh
    {
    public:
        const std::unique_ptr<const Cell[]> cells;
        const std::unique_ptr<const Face[]> faces;

        const int nCells;
        const int nFaces;

        Mesh(int nCellsIn, int nFacesIn, std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn);
 
        ~Mesh() {};
    };
}