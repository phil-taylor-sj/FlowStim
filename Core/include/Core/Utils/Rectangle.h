#pragma once

namespace fstim
{
    /**
     * \class Rectangle
     * \brief A basic data object storing the center point and x, y dimensions
     * of a rectangle.
     */
    template<typename T>
    class Rectangle
    {
    public:
        T center, lengths;

        /**
         * \brief Psuedo-default constructor for the class, with optional
         * initial value specifications.
         */
        Rectangle(T centerIn = T(), T lengthsIn = T()) : 
            center(centerIn), lengths(lengthsIn) {};
    };
}