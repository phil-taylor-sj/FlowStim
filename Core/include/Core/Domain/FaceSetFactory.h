#pragma once

#include <Core/Domain/Mesh.h>

namespace fstim
{
    class FaceSetFactory
    {
    public:
        void static fourWalls(Mesh& mesh);
    };
}