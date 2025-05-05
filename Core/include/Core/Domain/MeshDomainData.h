#pragma once

#include <memory>
#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Vertex.h>
#include <VecPlus/Vec2.h>

namespace fstim
{
    struct MeshDomainData {
        int nCells = 0;
        int nFaces = 0;
        int nVertices = 0;
        std::unique_ptr<Cell[]> cells = nullptr;
        std::unique_ptr<Face[]> faces = nullptr;
        std::unique_ptr<Vertex[]> vertices = nullptr;
        vecp::Vec2d length = vecp::Vec2d(0., 0.);
    };
}