#pragma once

#include <memory>
#include <span>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>
#include <Core/Domain/GridProfileEquidistant.h>
#include <Core/Domain/VertexMapping.h>
#include <Core/Domain/MeshDomainData.h>

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

namespace fstim
{
    template <typename D, typename F, typename I>
    class MeshFactory
    {
    public:   

        MeshFactory() {};
        
        virtual ~MeshFactory() {};

    protected:
        
        int m_nCells = 4;
        int m_nFaces = 4;
        int m_nVertices = 4;
        std::unique_ptr<Cell<D, F>[]> m_cells = nullptr;
        std::unique_ptr<Face<D>[]> m_faces = nullptr;
        std::unique_ptr<Vertex<D>[]> m_vertices = nullptr;
        D length {};

        void m_calcCellToCellSpacing(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells);

        void m_calcOwnerWeights(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells);

        void m_createMesh();
    };
    
    extern template class MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>;
}