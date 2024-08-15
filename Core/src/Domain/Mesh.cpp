#include <Core/Domain/Mesh.h>

namespace fstim
{
    int Mesh::getCellId(int i, int j)
    {
        return j * this->size.x + i;
    }

    Mesh::Mesh(Vec2i sizeIn, Vec2d lengthIn, Cell* cellsIn, Face* facesIn) 
        : size(sizeIn), length(lengthIn)
    {
        this->cells = std::const_cast<const Cell*>(cellsIn);
        this->faces = std::const_cast<const Face*>(facesIn); 
    
        this->nCells = this->size.x * this-> size.y;
        this->nFaces = (this->size.x + 1) * this->size.y
            + (this->size.y + 1) * this->size.x;
    }

    Mesh::~Mesh()
    {
        delete this->cells;
        delete this->faces;
    }
}