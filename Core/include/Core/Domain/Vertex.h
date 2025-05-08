#pragma once 

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>
#include <vector>

namespace fstim
{
    template <typename T>
    class Vertex
    {
    public:
        size_t vertexId = 0;

        T position{};

        std::vector<size_t> faceId = {};

        std::vector<size_t> cellId = {};

        std::vector<double> cellWeight = {};

        Vertex() : position(T()){}

        Vertex(T newPosition) : position(newPosition){}

        //Vertex2d(double newX, double newY) : position(vecp::Vec2d(newX, newY)){}
    };

    extern template class Vertex<vecp::Vec2d>;
    extern template class Vertex<vecp::Vec3d>;

    using Vertex2d = Vertex<vecp::Vec2d>;
    using Vertex3d = Vertex<vecp::Vec3d>;
}