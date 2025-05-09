#pragma once 

#include <stdexcept>
#include <format>

#include <VecPlus/Vec2.h>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>
#include <Core/Domain/GridProfileEquidistant.h>
#include <Core/Domain/VertexMapping.h>
#include <Core/Domain/MeshDomainData.h>

#include <Core/Domain/MeshFactory.h>


namespace fstim
{
    class MeshFactory2dStructured : public MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>
    {
    public:

        std::unique_ptr<Mesh2d> operator()(vecp::Vec2i size, vecp::Vec2d length);
        
        MeshFactory2dStructured() : m_profile(std::make_unique<GridProfileEquidistant>()) {};

        MeshFactory2dStructured(std::unique_ptr<GridProfile> profile) : m_profile(std::move(profile)) {};

        ~MeshFactory2dStructured() {};

    private:

        const std::unique_ptr<GridProfile> m_profile;

        const Compass m_directions[4] = {Compass::NORTH, Compass::SOUTH, Compass::EAST, Compass::WEST};

        int m_calcCellId(int i, int j, vecp::Vec2i size);
        
        vecp::Vec2i m_getCellLocations(int id, vecp::Vec2i size);

        void m_assignIds(std::unique_ptr<Cell2d[]>& cells, std::unique_ptr<Face2d[]>& faces, vecp::Vec2i size);

        void m_assignProperties(std::unique_ptr<Cell2d[]>& cells, std::unique_ptr<Face2d[]>& faces, vecp::Vec2i size, vecp::Vec2d length);
        
        void m_assignVertices(std::unique_ptr<Cell2d[]>& cells, std::unique_ptr<Face2d[]>& faces, vecp::Vec2i size);

    };
    
    extern template class MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>;

}