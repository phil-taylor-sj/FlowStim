#pragma once

#include <memory>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Vertex.h>
#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

namespace fstim
{
    // This struct is only intended to be used for passing the vast number 
    // of required arguments into the Mesh class constructor
    template <typename T, typename U>
    struct MeshDomainData {
        int nCells = 0;
        int nFaces = 0;
        int nVertices = 0;
        std::unique_ptr<Cell<T, U>[]> cells = nullptr;
        std::unique_ptr<Face<T>[]> faces = nullptr;
        std::unique_ptr<Vertex<T>[]> vertices = nullptr;
        T length = T();
    };

    template struct MeshDomainData<vecp::Vec2d, vecp::Vec2f>;
    template struct MeshDomainData<vecp::Vec3d, vecp::Vec3f>;

    using MeshDomainData2d = MeshDomainData<vecp::Vec2d, vecp::Vec2f>;
    using MeshDomainData3d = MeshDomainData<vecp::Vec3d, vecp::Vec3f>;
}