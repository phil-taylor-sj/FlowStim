#pragma once
#include <Core/Domain/GridProfile.h>

namespace fstim
{
    class GridProfileEquidistant : public GridProfile
    {
    public:
        std::tuple<std::vector<double>, std::vector<double>> operator()(int nCells, double length) const override;

        GridProfileEquidistant() {};

        ~GridProfileEquidistant() {};
    };
}