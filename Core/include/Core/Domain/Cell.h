#pragma once
#include <VecPlus/Vec2.h>
#include "Domain/Compass.h"

#include <map>

namespace fstim
{
    class Cell
    {
    public:
        int id;
        int i;
        int j;
        double volume;
        vecp::Vec2d width;
        vecp::Vec2d center;

        /**
         * @brief Holds the id's of each neighbouring cell.
         */
        std::map<Compass, int> neighId = {
            {Compass.NORTH, -1},
            {Compass.SOUTH, -1},
            {Compass.EAST, -1},
            {Compass.WEST, -1}
        };

        /**
         * @brief Holds the id's of each surrounding face cell.
         */
        std::map<Compass, int> faceId = {
            {Compass.NORTH, -1},
            {Compass.SOUTH, -1},
            {Compass.EAST, -1},
            {Compass.WEST, -1}
        };

        Cell(int id, int iLoc, int jLoc);

        ~Cell();
    }
}