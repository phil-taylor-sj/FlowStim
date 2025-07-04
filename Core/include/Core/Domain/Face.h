#pragma once

#include <VecPlus/Vec2.h>
#include <VecPlus/Vec3.h>

#include <vector>
#include <map>

namespace fstim
{
    /**
     * \class Face
     * \brief Represents a geometric face in a 2D space, with properties such as area, center, and boundary status.
     *
     * The Face class encapsulates the properties and behavior of a geometric face, including its unique identifier,
     * area, center position, boundary status, and associated cell identifiers. It provides a constructor to initialize
     * the face with a given ID and a destructor for cleanup.
     */
    template <typename T>
    class Face
    {
    public:
        /**
         * \brief The unique identifier for the face.
         *
         * This ID is constant and must be initialized at the time of object creation.
         */
        int id, ownerId, neighId;

        /**
         * \brief The area of the face.
         *
         * Represents the surface area of the face in 2D space.
         */
        T normal{}; 

        /**
         * \brief The center of the face.
         *
         * A Vec2d object representing the coordinates of the center of the face.
         */
        T center{};

        double cellToCellSpacing {1.0};

        double ownerWeight = 1.;

        /**
         * \brief Indicates whether the face is on the boundary.
         *
         * A boolean flag that is true if the face is a boundary face, false otherwise.
         */
        bool isBoundary;

        /**
         * 
         */
        std::vector<size_t> vertexId = {0, 0};

        /**
         * \brief Constructor that initializes the Face with a given ID.
         */
        Face() : id(-1), ownerId(-1), neighId(-1) {};

        /**
         * \attention Destructor for the Face class.
         *
         * Cleans up any resources used by the Face object.
         */
        ~Face() {};
    };

    extern template class Face<vecp::Vec2d>;
    extern template class Face<vecp::Vec3d>;

    using Face2d = Face<vecp::Vec2d>;
    using Face3d = Face<vecp::Vec3d>;
}