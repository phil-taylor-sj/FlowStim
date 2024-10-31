#pragma once

#include <VecPlus/Vec2.h>

#include <Core/Domain/Vertex.h>
#include <functional>

#include <Core/Domain/Mesh.h>

namespace fstim
{
    struct VertexKey
    {
        vecp::Vec2i quantPosition{};
        bool operator==(const VertexKey& key) const {
            return this->quantPosition.x == key.quantPosition.x 
                && this->quantPosition.y == key.quantPosition.y ;
        }
    };
}

namespace std {
    template<>
    struct hash<fstim::VertexKey> {
        size_t operator()(const fstim::VertexKey& key) const {
            size_t h1 = hash<int>{}(key.quantPosition.x);
            size_t h2 = hash<int>{}(key.quantPosition.y);
            return h1 ^ (h2 << 1);
        }
    };
}

namespace fstim
{
    class VertexMapping
    {
    public:
        static VertexKey quantisePosition(const Vertex& v, double gridSize);
    
        static std::vector<Vertex> createVertices(Cell* cells, size_t nCells, Face* faces, size_t nFaces);

    private:
        static std::vector<Vertex> m_getAllVertices(Face* faces, int nFaces);

        static std::vector<Vertex> m_getUniqueVertices(std::vector<Vertex>& allVertices, std::vector<size_t>& indexMapping);
    
        static void m_assignFaceVertexIds(Face* faces, size_t nFaces, const std::vector<size_t>& indexMapping);

        static void m_assignCellVertexIds(Cell* cells, size_t nCells, const Face* faces);

        static void m_assignVertexFaceIds(const Face* faces, size_t nFaces, std::vector<Vertex>& vertices);

        static void m_assignVertexCellIds(const Cell* cells, size_t nCells, std::vector<Vertex>& vertices);
    };
}