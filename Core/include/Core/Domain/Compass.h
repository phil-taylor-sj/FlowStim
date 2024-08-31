#pragma once

namespace fstim
{

    /**
    * \enum fstim::Compass
    * \brief Enumeration representing the four cardinal directions.
    *
    * This enumeration is used to specify the axial direction.
    *
    * \var fstim::Compass::NORTH
    * Represents the negative direction parallel to the y-axis.
    *
    * \var fstim::Compass::SOUTH
    * Represents the positive direction parallel to the y-axis.
    *
    * \var fstim::Compass::EAST
    * Represents the positive direction parallel to the x-axis.
    *
    * \var fstim::Compass::WEST
    * Represents the negative direction parallel to the x-axis.
    */
    enum class Compass
    {
        NORTH = 0,
        SOUTH = 1,
        EAST = 2,
        WEST = 3
    };
}