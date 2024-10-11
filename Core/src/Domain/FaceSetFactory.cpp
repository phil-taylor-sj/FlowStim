#include <Core/Domain/FaceSetFactory.h>

namespace fstim
{
    void FaceSetFactory::fourWalls(Mesh& mesh)
    {
        vecp::Vec2d length = mesh.length; 
        // South
        mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, 0.), vecp::Vec2d(length.x, 0.001));
        // North
        mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, length.y), vecp::Vec2d(length.x, 0.001));

        mesh.addFaceSet(vecp::Vec2d(0., 0.5 * length.y), vecp::Vec2d(0.001, length.y));
        // North
        mesh.addFaceSet(vecp::Vec2d(length.x, 0.5 * length.y), vecp::Vec2d(0.00, length.y));
    }
}