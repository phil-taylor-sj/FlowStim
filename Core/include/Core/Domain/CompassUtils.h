#pragma once

#include <Core/Domain/Compass.h>

#include <unordered_map>

namespace fstim 
{
    class CompassUtils
    {
    public:
        static Compass getReverseDirection(Compass direction);

    private:
        static const std::unordered_map<Compass, Compass> m_reversedDirections;
    };
}