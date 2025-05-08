#pragma once
#include <vector>
#include <set>

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>
#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Vertex.h>

#include <Core/Domain/MeshDomainData.h>

namespace fstim
{
    /**
     * \class Mesh
     * \brief A collection of cells, faces and face sets which make up a full mesh.
     * 
     * 
     * \param cells 
     * A unique pointer to a C-style array of cell instances.
     * 
     * \param faces
     * A unique pointer to a C-style array of face instances.  
     * 
     * \param vertices
     * A unique pointer to a C-style array of vertex instances.  
     * 
     * \param nCells;
     * Total number of cells in the cells array.
     * 
     * \param nFaces;
     * Total number of faces in the faces array.
     * 
    * \param nVertices;
     * Total number of vertices in the vertices array.
     * 
     * \param length
     * The dimensions of the domain in the x and y directions.
     * 
     * \param m_faceSets
     * An std::vector array contianing sets of faceIds which belong to the face set
     * with an id denoted by the index.
     */
    template <typename T, typename U>
    class Mesh
    {
    public:
        const std::unique_ptr<const Cell<T, U>[]> cells;
        const std::unique_ptr<const Face<T>[]> faces;
        const std::unique_ptr<const Vertex<T>[]> vertices;

        const int nCells;
        const int nFaces;
        const int nVertices;
        const T length;

        /**
         * \brief Add a new face set, by selecting faces based on a single 2d rectangle.
         * 
         * The method takes a 2d rectangular geometry, and assigns face ids to
         * the set if they lie within the geometry bounds.
         * 
         * \param center The x-y coordinates of the rectangle centre.
         * 
         * \param lengths The length (x) and width of (y) of the rectangle.
         * 
         * \return The id of the new face set.
         */
        int addFaceSet(T center, T lengths);
        
         /**
         * \brief Add a new face set, by selecting faces based on multiple 2d rectangles.
         * 
         * The method takes an array of 2d rectangular geometries, and assigns face ids to
         * the set if they lie within the bounds of any given geometry.
         * 
         * \param  boundaries An array of tuples, each contianing the x-y coordinates of the 
         * rectangle centre and the length (x) and width of (y) of the rectangle.
         * 
         * \return The id of the new face set.
         */
        int addFaceSet(std::vector<std::tuple<T, T>> boundaries);

        /**
         * \brief Get the id of a face set which contains a face with the specified id.
         * 
         * \param faceId The value of the face id to search for.
         * 
         * \return The id of the face set, or -1 if the face id is not found.
         */
        int getFaceSetId(int faceId) const;

        Mesh(MeshDomainData<T, U>& data);
 
        virtual ~Mesh() = default;
    
    private:
        std::vector<std::set<int>> m_faceSets{};
    
        void m_addFacesToSet(std::set<int>& newSet, T center, T lengths);

    };

    extern template class Mesh<vecp::Vec2d, vecp::Vec2f>;
    extern template class Mesh<vecp::Vec3d, vecp::Vec3f>;

    using Mesh2d = Mesh<vecp::Vec2d, vecp::Vec2f>;
    using Mesh3d = Mesh<vecp::Vec3d, vecp::Vec3f>;
}