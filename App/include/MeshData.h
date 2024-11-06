#pragma once

#include <vector>

#include <VecPlus/Vec2.h>


class MeshData
{
public:
    int nCells = 1;
    vecp::Vec2f length = vecp::Vec2f(1., 1.);
    std::vector<vecp::Vec2f> vertices = {};
    std::vector<std::vector<size_t>> cellElements = {};
    unsigned meshId = 0;
};
