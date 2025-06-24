#pragma once

#include <VecPlus/Vec2.h>

#include <Core/Domain/Vertex.h>
#include <functional>
#include <iostream>
#include <cmath> 

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
        static VertexKey quantisePosition(const Vertex2d& v, double gridSize);
    
        static std::vector<Vertex2d> createVertices(Cell2d* cells, size_t nCells, Face2d* faces, size_t nFaces);

    private:
        static std::vector<Vertex2d> m_getAllVertices(Face2d* faces, int nFaces);

        static std::vector<Vertex2d> m_getUniqueVertices(std::vector<Vertex2d>& allVertices, std::vector<size_t>& indexMapping);
    
        static void m_assignFaceVertexIds(Face2d* faces, size_t nFaces, const std::vector<size_t>& indexMapping);

        static void m_assignCellVertexIds(Cell2d* cells, size_t nCells, const Face2d* faces);

        static void m_assignVertexFaceIds(const Face2d* faces, size_t nFaces, std::vector<Vertex2d>& vertices);

        static void m_assignVertexCellIds(const Cell2d* cells, size_t nCells, std::vector<Vertex2d>& vertices);
    };
}