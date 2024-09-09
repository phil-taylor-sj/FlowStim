#include <Core/Domain/Mesh.h>

namespace fstim
{
    
    int Mesh::addFaceSet(vecp::Vec2d center, double width, double height)
    {
        std::set<int> newSet;
        for (int id; id < nFaces; id++)
        {
            vecp::Vec2d offset = faces[id].center - center;
            if (std::abs(offset.x) <= 0.5 * width ||
                std::abs(offset.y) <= 0.5 * height)
                {
                    newSet.insert(id);
                }
           
        }
        this->m_faceSets.push_back(std::move(newSet));
        return m_faceSets.size() - 1;
    }

    int Mesh::getFaceSetId(int faceId) const
    {
        for (int setId = 0; setId < m_faceSets.size(); setId++)
        {
            if (this->m_faceSets[setId].count(faceId) == 1)
            {
                return setId;
            }
        }
        return -1;
    }

    Mesh::Mesh(int nCellsIn, int nFacesIn,  std::unique_ptr<Cell[]> cellsIn, std::unique_ptr<Face[]> facesIn, vecp::Vec2f lengthIn) 
        : nCells(nCellsIn), nFaces(nFacesIn), cells(std::move(cellsIn)), faces(std::move(facesIn)), length(lengthIn)
    {
        m_faceSets.resize(0);        
    }
}