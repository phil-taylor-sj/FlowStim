#include <Core/Domain/FaceSetFactory.h>

namespace fstim
{
    void FaceSetFactory::fourWalls(Mesh& mesh)
    {
        vecp::Vec2d length = mesh.length;
        defineNewFaceSetByCompass(mesh, Compass::NORTH);
        defineNewFaceSetByCompass(mesh, Compass::SOUTH);
        defineNewFaceSetByCompass(mesh, Compass::WEST);
        defineNewFaceSetByCompass(mesh, Compass::EAST);
        // North
        //mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, length.y), vecp::Vec2d(length.x, 0.001));
        // South
        //mesh.addFaceSet(vecp::Vec2d(0.5 * length.x, 0.), vecp::Vec2d(length.x, 0.001));
        // West
        //mesh.addFaceSet(vecp::Vec2d(0., 0.5 * length.y), vecp::Vec2d(0.001, length.y));
        // East
        //mesh.addFaceSet(vecp::Vec2d(length.x, 0.5 * length.y), vecp::Vec2d(0.001, length.y));
    }

    int FaceSetFactory::defineNewFaceSetByCompass(Mesh& mesh, Compass direction)
    {
        vecp::Vec2d length = mesh.length;

        std::map<Compass, vecp::Vec2d> location = {
            {Compass::NORTH, vecp::Vec2d(0.5 * length.x, length.y)},
            {Compass::SOUTH, vecp::Vec2d(0.5 * length.x, 0.)},
            {Compass::WEST, vecp::Vec2d(0., 0.5 * length.y)},
            {Compass::EAST, vecp::Vec2d(length.x, 0.5 * length.y)}
        };

        std::map<Compass, vecp::Vec2d> region = {
            {Compass::NORTH, vecp::Vec2d(length.x, 0.001)},
            {Compass::SOUTH, vecp::Vec2d(length.x, 0.001)},
            {Compass::WEST, vecp::Vec2d(0.001, length.y)},
            {Compass::EAST, vecp::Vec2d(0.001, length.y)}
        };

        // mesh.addFaceSet returns the id of the new face set.
        return mesh.addFaceSet(location.at(direction), region.at(direction));
    }
}