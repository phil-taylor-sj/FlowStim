#pragma once

#include <memory>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Vertex.h>
#include <VecPlus/Vec2.h>

namespace fstim
{
    // This struct is only intended to be used for passing the vast number 
    // of required arguments into the Mesh class constructor
    struct MeshDomainData {
        int nCells = 0;
        int nFaces = 0;
        int nVertices = 0;
        std::unique_ptr<Cell2d[]> cells = nullptr;
        std::unique_ptr<Face2d[]> faces = nullptr;
        std::unique_ptr<Vertex[]> vertices = nullptr;
        vecp::Vec2d length = vecp::Vec2d(0., 0.);
    };
}