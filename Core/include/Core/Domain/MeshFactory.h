#pragma once 

#include <VecPlus/Vec2.h>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>
#include <Core/Domain/GridProfileEquidistant.h>

using namespace vecp;

namespace fstim
{
    class MeshFactory
    {
    public:
        std::unique_ptr<Mesh> operator()(Vec2i size, Vec2d length);
        
        MeshFactory() : m_profile(std::make_unique<GridProfileEquidistant>()) {};

        MeshFactory(std::unique_ptr<GridProfile> profile) : m_profile(std::move(profile)) {};

        ~MeshFactory() {};

    private:

        const std::unique_ptr<GridProfile> m_profile;

        const Compass m_directions[4] = {Compass::NORTH, Compass::SOUTH, Compass::EAST, Compass::WEST};

        int m_calcCellId(int i, int j, Vec2i size);
        
        Vec2i m_getCellLocations(int id, Vec2i size);

        void m_assignIds(std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, Vec2i size);

        void m_assignProperties(std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, Vec2i size, Vec2d length);
        
        void m_assignVertices(std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, vecp::Vec2i size);

    };
    
}