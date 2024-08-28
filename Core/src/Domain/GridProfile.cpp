#include <Core/Domain/GridProfile.h>

namespace fstim
{
    std::tuple<std::vector<double>, std::vector<double>> 
    GridProfile::m_calcCellProperties(std::vector<double> faceCenters) const
    {
        int nCells = faceCenters.size() - 1;
        std::vector<double> cellCenters(nCells);
        std::vector<double> cellWidths(nCells);
        for (int i = 0; i < faceCenters.size() - 1; i++)
        {
            double faceBefore = faceCenters[i];
            double faceAfter = faceCenters[i + 1];
            cellCenters[i] = 0.5 * (faceCenters[i] + faceCenters[i + 1]);
            cellWidths[i] = faceCenters[i + 1] - faceCenters[i];
        }
        return std::move(
            std::make_tuple<>(std::move(cellCenters), std::move(cellWidths))
        );
    }
   
    GridProfile::GridProfile()
    {

    }
    
    GridProfile::~GridProfile()
    {

    }

    
}