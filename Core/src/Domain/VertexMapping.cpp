#include <Core/Domain/VertexMapping.h>

namespace fstim
{
    VertexKey VertexMapping::quantisePosition(const Vertex2d& vertex, double gridSize) {
        return VertexKey {
            vecp::Vec2i(
                static_cast<int>(std::floor(vertex.position.x / gridSize + 0.5)),
                static_cast<int>(std::floor(vertex.position.y / gridSize + 0.5))
            )
        };
    }

    std::vector<Vertex2d> VertexMapping::createVertices(Cell2d* cells, size_t nCells, Face2d* faces, size_t nFaces)
    {
        std::vector<Vertex2d> allVertices = VertexMapping::m_getAllVertices(faces, nFaces);
        std::vector<size_t> indexMapping(allVertices.size());
        
        std::vector<Vertex2d> vertices = VertexMapping::m_getUniqueVertices(allVertices, indexMapping);

        VertexMapping::m_assignFaceVertexIds(faces, nFaces, indexMapping);

        VertexMapping::m_assignCellVertexIds(cells, nCells, faces);

        VertexMapping::m_assignVertexFaceIds(faces, nFaces, vertices);

        VertexMapping::m_assignVertexCellIds(cells, nCells, vertices);

        return std::move(vertices);
    }

    std::vector<Vertex2d> VertexMapping::m_getAllVertices(Face2d* faces, int nFaces)
    {
        std::vector<Vertex2d> vertices(nFaces * 2);
        for (int faceId = 0; faceId < nFaces; faceId++)
        {
                Face2d& face = faces[faceId];
                vecp::Vec2d tangent = (face.normal.rotate(90.)) * 0.5;
                
                face.vertexId[0] = faceId * 2;
                face.vertexId[1] = faceId * 2 + 1;
                
                vertices[face.vertexId[0]].position = face.center + tangent;
                vertices[face.vertexId[1]].position = face.center - tangent;

        }
        return std::move(vertices);
    }

    std::vector<Vertex2d> VertexMapping::m_getUniqueVertices(std::vector<Vertex2d>& allVertices, std::vector<size_t>& indexMapping )
    {
        std::unordered_map<VertexKey, size_t> vertexMap;
        std::vector<Vertex2d> uniqueVertices;
        for (size_t i = 0; i < allVertices.size(); i++) 
        {
            Vertex2d& vertex = allVertices[i];
            VertexKey key = VertexMapping::quantisePosition(vertex, 0.00001);

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

    void VertexMapping::m_assignFaceVertexIds(Face2d* faces, size_t nFaces, const std::vector<size_t>& indexMapping)
    {
       for (int faceId = 0; faceId < nFaces; faceId++)
       {
            Face2d& face = faces[faceId];
            face.vertexId[0] = indexMapping[face.vertexId[0]];
            face.vertexId[1] = indexMapping[face.vertexId[1]];
       }
    }

    void VertexMapping::m_assignCellVertexIds(Cell2d* cells, size_t nCells, const Face2d* faces)
    {
       for (size_t cellId = 0; cellId < nCells; cellId++)
       {
            Cell2d& cell = cells[cellId];
            std::vector<size_t> cornerIds = {};
            for (int faceId : cell.faceId)
            {
                const Face2d& face = faces[faceId];
                cornerIds.push_back((cellId == face.ownerId) ? face.vertexId[0] : face.vertexId[1]);
            }
            cell.vertexId = cornerIds;
       }
    }

    void VertexMapping::m_assignVertexFaceIds(const Face2d* faces, size_t nFaces, std::vector<Vertex2d>& vertices)
    {
        for (size_t faceId = 0; faceId < nFaces; faceId++)
        {
            for (size_t vertexId : faces[faceId].vertexId)
            {
                vertices[vertexId].faceId.push_back(faceId);
            }
        }
    }

    void VertexMapping::m_assignVertexCellIds(const Cell2d* cells, size_t nCells, std::vector<Vertex2d>& vertices)
    {
        for (size_t cellId = 0; cellId < nCells; cellId++)
        {
            for (size_t vertexId : cells[cellId].vertexId)
            {
                vertices[vertexId].cellId.push_back(cellId);
            }
        }

       for (Vertex2d& vertex : vertices)
       {
            double total = 0.;
            for (size_t cellId : vertex.cellId)
            {   
                total += vertex.position.mag(cells[cellId].center);
            }

            for (size_t cellId : vertex.cellId)
            {
                double weight = vertex.position.mag(cells[cellId].center) / total;
                vertex.cellWeight.push_back(weight);
            }
       }
    }
}