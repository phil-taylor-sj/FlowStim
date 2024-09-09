#pragma once
#include <vector>
#include <set>

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
        const vecp::Vec2f length;

        int addFaceSet(vecp::Vec2d center, double width, double height);
        
        int getFaceSetId(int faceId) const;

        Mesh(int nCellsIn, int nFacesIn, std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn, vecp::Vec2f lengthIn);
 
        virtual ~Mesh() = default;
    
    private:
        std::vector<std::set<int>> m_faceSets{};
    
    };
}