#include <Core/Domain/VertexMapping.h>

namespace fstim
{
    VertexKey VertexMapping::quantisePosition(const Vertex& vertex, double gridSize) {
        return VertexKey {
            vecp::Vec2i(
                static_cast<int>(vertex.position.x / gridSize),
                static_cast<int>(vertex.position.y / gridSize)
            )
        };
    }

    std::vector<Vertex> VertexMapping::createVertices(Cell* cells, size_t nCells, Face* faces, size_t nFaces)
    {
        std::vector<Vertex> allVertices = VertexMapping::m_getAllVertices(faces, nFaces);
        std::vector<size_t> indexMapping(allVertices.size());
        
        std::vector<Vertex> vertices = VertexMapping::m_getUniqueVertices(allVertices, indexMapping);
    
        VertexMapping::m_assignFaceVertexIds(faces, nFaces, indexMapping);

        VertexMapping::m_assignCellVertexIds(cells, nCells, faces);

        VertexMapping::m_assignVertexFaceIds(faces, nFaces, vertices);

        VertexMapping::m_assignVertexCellIds(cells, nCells, vertices);

        return std::move(vertices);
    }

    std::vector<Vertex> VertexMapping::m_getAllVertices(Face* faces, int nFaces)
    {
        std::vector<Vertex> vertices(nFaces * 2);
        for (int faceId = 0; faceId < nFaces; faceId++)
        {
                const Face& face = faces[faceId];
                vecp::Vec2d tangent = (face.normal.rotate(90.)) * 0.5;
                
                faces->vertexId[0] = faceId * 2;
                faces->vertexId[1] = faceId * 2 + 1;
                
                vertices[faces->vertexId[0]].position = face.center + tangent;
                vertices[faces->vertexId[1]].position = face.center - tangent;

        }
        return std::move(vertices);
    }

    std::vector<Vertex> VertexMapping::m_getUniqueVertices(std::vector<Vertex>& allVertices, std::vector<size_t>& indexMapping )
    {
        std::unordered_map<VertexKey, size_t> vertexMap;
        std::vector<Vertex> uniqueVertices;
        for (size_t i = 0; i < allVertices.size(); ++i) 
        {
            Vertex& vertex = allVertices[i];
            VertexKey key = VertexMapping::quantisePosition(vertex, 0.0001);

            auto it = vertexMap.find(key);
            if (it != vertexMap.end()) {
                size_t uniqueIndex = it->second;
                indexMapping[i] = uniqueIndex;
            } 
            else 
            {
                size_t uniqueIndex = uniqueVertices.size();
                vertex.vertexId = uniqueIndex;
                uniqueVertices.push_back(vertex);
                vertexMap[key] = uniqueIndex;
                indexMapping[i] = uniqueIndex;
            }
        }
        return std::move(uniqueVertices);
    }

    void VertexMapping::m_assignFaceVertexIds(Face* faces, size_t nFaces, const std::vector<size_t>& indexMapping)
    {
       for (int faceId = 0; faceId < nFaces; faceId++)
       {
            Face& face = faces[faceId];
            face.vertexId[0] = indexMapping[face.vertexId[0]];
            face.vertexId[1] = indexMapping[face.vertexId[1]];
       }
    }

    void VertexMapping::m_assignCellVertexIds(Cell* cells, size_t nCells, const Face* faces)
    {
       for (int cellId = 0; cellId < nCells; cellId++)
       {
            Cell& cell = cells[cellId];
            std::vector<size_t> cornerIds = {};
            for (int faceId : cells->faceId)
            {
                const Face& face = faces[faceId];
                cornerIds.push_back((cellId == face.ownerId) ? face.vertexId[0] : face.vertexId[1]);
            }
            cell.vertexId = cornerIds;
       }
    }

    void VertexMapping::m_assignVertexFaceIds(const Face* faces, size_t nFaces, std::vector<Vertex>& vertices)
    {
        for (size_t faceId = 0; faceId < nFaces; faceId++)
        {
            for (size_t vertexId : faces[faceId].vertexId)
            {
                vertices[vertexId].faceId.push_back(faceId);
            }
        }
    }

    void VertexMapping::m_assignVertexCellIds(const Cell* cells, size_t nCells, std::vector<Vertex>& vertices)
    {
        for (size_t cellId = 0; cellId < nCells; cellId++)
        {
            for (size_t vertexId : cells[cellId].vertexId)
            {
                vertices[vertexId].cellId.push_back(cellId);
            }
        }
    }


}