#pragma once

#include <memory>

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
        
        void m_calcCellToCellSpacing(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells) {};

        void m_calcOwnerWeights(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells) {};
    };
    
    extern template class MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>;
}