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
        const Vec2i size;
        const Vec2d length;
        const Cell* const cells;
        const Face* const faces;

        const int nCells;
        const int nFaces;

        int getCellId(int i, int j);

        Mesh(Vec2i sizeIn, Vec2d lengthIn, Cell* cellsIn. Face* facesIn) 
            : size(sizeIn), length(lengthIn);
 
        ~Mesh();
    }
}