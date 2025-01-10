#include <Core/Domain/FaceSetFactory.h>

namespace fstim
{
    void FaceSetFactory::fourWalls(Mesh& mesh)
    {
        vecp::Vec2d length = mesh.length;
        // North
        mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, length.y), vecp::Vec2d(length.x, 0.001));
        // South
        mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, 0.), vecp::Vec2d(length.x, 0.001));
        // West
        mesh.addFaceSet(vecp::Vec2d(0., 0.5 * length.y), vecp::Vec2d(0.001, length.y));
        // East
        mesh.addFaceSet(vecp::Vec2d(length.x, 0.5 * length.y), vecp::Vec2d(0.001, length.y));
    }
}