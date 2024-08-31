#include <Core/Domain/GridProfileEquidistant.h>

namespace fstim
{
    std::tuple<std::vector<double>, std::vector<double>> GridProfileEquidistant::operator()(int nCells, double length) const
    {
        std::vector<double> faceCenters(nCells + 1);
        for (int i = 0; i < faceCenters.size(); i++)
        {
            faceCenters[i] = (double) i * length / nCells;
        }
        return std::move(this->m_calcCellProperties(std::move(faceCenters)));
    }
}