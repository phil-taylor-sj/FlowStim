#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>

namespace fstim
{
    class FaceSetFactory
    {
    public:
        void static fourWalls(Mesh& mesh);

        int static defineNewFaceSetByCompass(Mesh& mesh, Compass direction);
    };
}