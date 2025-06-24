#include <Core/Domain/Mesh.h>

#include <iostream>

namespace fstim
{
    template <typename T, typename U>
    int Mesh<T, U>::addFaceSet(T center, T lengths)
    {
        std::set<int> newSet;
        this->m_addFacesToSet(newSet, center, lengths);
        this->m_faceSets.push_back(std::move(newSet));
        return m_faceSets.size() - 1;
    }

    template <typename T, typename U>
    int Mesh<T, U>::addFaceSet(std::vector<std::tuple<T, T>> boundaries)
    {
        std::set<int> newSet;
        for (std::tuple<T, T> boundary : boundaries)
        {
            this->m_addFacesToSet(newSet, std::get<0>(boundary), std::get<1>(boundary));
        }
        
        this->m_faceSets.push_back(std::move(newSet));
        return m_faceSets.size() - 1;
    }

    template <typename T, typename U>
    void Mesh<T, U>::m_addFacesToSet(std::set<int>& newSet, T center, T lengths)
    {
        for (int id = 0; id < nFaces; id++)
        {
            // TODO: Create function which can be used for either 2 or 3 dimensions.
            T offset = (faces[id].center - center).abs();
            T ratio = offset / (lengths * 0.5);         

            if (ratio.max() <= 1.0)
            {
                newSet.insert(id);
            }
        }
    }

    template <typename T, typename U>
    int Mesh<T, U>::getFaceSetId(int faceId) const
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

    template <typename T, typename U>
    Mesh<T, U>::Mesh(MeshDomainData<T, U>& meshData) 
        : nCells(meshData.nCells), nFaces(meshData.nFaces), nVertices(meshData.nVertices), 
        cells(std::move(meshData.cells)), faces(std::move(meshData.faces)), vertices(std::move(meshData.vertices)),
        length(meshData.length)
    {
        m_faceSets.resize(0);        
    }

    template class Mesh< vecp::Vec2d, vecp::Vec2f >;
    template class Mesh< vecp::Vec3d, vecp::Vec3f >;
}