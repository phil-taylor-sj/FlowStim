#include <Core/Domain/Mesh.h>

#include <iostream>

namespace fstim
{
    int Mesh2d::addFaceSet(vecp::Vec2d center, vecp::Vec2d lengths)
    {
        std::set<int> newSet;
        this->m_addFacesToSet(newSet, center, lengths);
        this->m_faceSets.push_back(std::move(newSet));
        return m_faceSets.size() - 1;
    }

    int Mesh2d::addFaceSet(std::vector<std::tuple<vecp::Vec2d, vecp::Vec2d>> boundaries)
    {
        std::set<int> newSet;
        for (std::tuple<vecp::Vec2d, vecp::Vec2d> boundary : boundaries)
        {
            this->m_addFacesToSet(newSet, std::get<0>(boundary), std::get<1>(boundary));
        }
        
        this->m_faceSets.push_back(std::move(newSet));
        return m_faceSets.size() - 1;
    }

    void Mesh2d::m_addFacesToSet(std::set<int>& newSet, vecp::Vec2d center, vecp::Vec2d lengths)
    {
        for (int id = 0; id < nFaces; id++)
        {
            vecp::Vec2d offset = faces[id].center - center;
            if (std::abs(offset.x) <= 0.5 * lengths.x &&
                std::abs(offset.y) <= 0.5 * lengths.y)
                {
                    newSet.insert(id);
                }
           
        }
    }

    int Mesh2d::getFaceSetId(int faceId) const
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

    Mesh2d::Mesh2d(MeshDomainData& meshData) 
        : nCells(meshData.nCells), nFaces(meshData.nFaces), nVertices(meshData.nVertices), 
        cells(std::move(meshData.cells)), faces(std::move(meshData.faces)), vertices(std::move(meshData.vertices)),
        length(meshData.length)
    {
        m_faceSets.resize(0);        
    }
}