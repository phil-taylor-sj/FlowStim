#include <Core/Domain/CompassUtils.h>


namespace fstim
{
    Compass CompassUtils::getReverseDirection(Compass direction)
    {
        return (m_reversedDirections.count(direction) != 0)
            ? m_reversedDirections.at(direction)
            : Compass::NONE;
    }

    const std::unordered_map<Compass, Compass> CompassUtils::m_reversedDirections = {
        {Compass::NORTH, Compass::SOUTH},
        {Compass::SOUTH, Compass::NORTH},
        {Compass::EAST,  Compass::WEST },
        {Compass::WEST,  Compass::EAST }
    };
}