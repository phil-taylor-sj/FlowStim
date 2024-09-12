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

        int addFaceSet(vecp::Vec2d center,vecp::Vec2d lengths);
        
        int addFaceSet(std::vector<std::tuple<vecp::Vec2d, vecp::Vec2d>> boundaries);

        int getFaceSetId(int faceId) const;

        Mesh(int nCellsIn, int nFacesIn, std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn, vecp::Vec2f lengthIn);
 
        virtual ~Mesh() = default;
    
    private:
        std::vector<std::set<int>> m_faceSets{};
    
        void m_addFacesToSet(std::set<int>& newSet, vecp::Vec2d center, vecp::Vec2d lengths);

    };
}