#pragma once 

#include <VecPlus/Vec2.h>
#include <vector>

namespace fstim
{
    class Vertex
    {
    public:
        size_t vertexId = 0;

        vecp::Vec2d position{};

        std::vector<size_t> faceId = {};

        std::vector<size_t> cellId = {};

        Vertex() : position(vecp::Vec2d(0., 0.)){}

        Vertex(vecp::Vec2d newPosition) : position(newPosition){}

        Vertex(double newX, double newY) : position(vecp::Vec2d(newX, newY)){}
    };
}