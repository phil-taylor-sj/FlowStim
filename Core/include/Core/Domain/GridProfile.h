#pragma once

#include <tuple>
#include <vector>

namespace fstim
{
    class GridProfile
    {
    public:
        virtual std::tuple<std::vector<double>, std::vector<double>> operator()(int nCells, double length) const = 0;

        GridProfile();

        virtual ~GridProfile();

    protected:
        std::tuple<std::vector<double>, std::vector<double>> m_calcCellProperties(std::vector<double> faceCenters) const; 

    };
}