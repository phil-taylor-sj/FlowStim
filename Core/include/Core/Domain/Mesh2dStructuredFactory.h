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
    class Mesh2dStructuredFactory : public MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>
    {
    public:

        [[nodiscard]] std::unique_ptr<Mesh2d> operator()(vecp::Vec2i size, vecp::Vec2d length);
        
        Mesh2dStructuredFactory() : m_profile(std::make_unique<GridProfileEquidistant>()) {};

        Mesh2dStructuredFactory(std::unique_ptr<GridProfile> profile) : m_profile(std::move(profile)) {};

        ~Mesh2dStructuredFactory() {};

    private:

        const std::unique_ptr<GridProfile> m_profile;

        const Compass m_directions[4] = {Compass::NORTH, Compass::SOUTH, Compass::EAST, Compass::WEST};

        int m_calcCellId(int i, int j, vecp::Vec2i size);
        
        vecp::Vec2i m_getCellLocations(int id, vecp::Vec2i size);

        void m_assignIds(vecp::Vec2i size);

        void m_assignProperties(vecp::Vec2i size, vecp::Vec2d length);
        
        void m_assignVertices(vecp::Vec2i size);

    };
    
    extern template class MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>;

}