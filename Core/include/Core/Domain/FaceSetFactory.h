#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>

namespace fstim
{
    class FaceSetFactory
    {
    public:
        void static fourWalls(Mesh2d& mesh);

        int static defineNewFaceSetByCompass(Mesh2d& mesh, Compass direction);
    };
}