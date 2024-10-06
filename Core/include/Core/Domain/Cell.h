#pragma once
#include <VecPlus/Vec2.h>

#include <map>

namespace fstim
{

    /**
     * \class Cell
     * \brief Represents a cell within the simulation domain.
     * 
     * The Cell class encapsulates the properties and attributes
     * associated with a single cell, including its ID, volume,
     * width, center position, and the IDs of the faces that make up 
     * the cell's boundaries.
     */
    class Cell
    {
    public:

        /**
         * \brief Unique identifier for the cell.
         * 
         * The ID is used to differentiate between different cells in the domain.
         * By default, the ID is initialized to -1.
         */
        int id = -1;

        /**
         * \brief Volume of the cell.
         * 
         * This represents the physical or computational volume of the cell.
         * The default value is 1.0.
         */
        double volume{1.};

        /**
         * \brief Center position of the cell in 2D space.
         * 
         * The center is represented as a 2D vector, which indicates the cell's
         * central position in both the x and y directions. Initialized to (0.0, 0.0).
         */
        vecp::Vec2d center{0., 0.};

        /**
         * \brief Identifiers for the faces of the cell.
         * 
         * A vector containing the IDs of the faces that form the boundaries of the cell.
         * By default, this vector is initialized with three elements, all set to -1.
         */
        std::vector<int> faceId = {-1, -1, -1, -1};

        /**
         * \brief Identifiers for the faces of the cell.
         * 
         * A vector containing the IDs of the faces that form the boundaries of the cell.
         * By default, this vector is initialized with three elements, all set to -1.
         */
        std::vector<vecp::Vec2f> vertices = {vecp::Vec2f(0.f, 0.f)};

        /**
         * \brief Default constructor.
         * 
         * Initializes a cell with default values.
         */
        Cell() : id(-1) {};

        /**
         * \brief Parameterized constructor.
         * 
         * Initializes a cell with the provided cell ID, iLoc, and jLoc.
         * Currently, the constructor sets the cell's ID to -1 by default.
         * 
         * \param cellId The ID to be assigned to the cell.
         * \param iLoc The i-location of the cell (not currently used).
         * \param jLoc The j-location of the cell (not currently used).
         */
        Cell(int cellId) : id(cellId) {};

        /**
         * \brief Destructor.
         * 
         * Cleans up any resources used by the cell, if necessary.
         */
        ~Cell() {};
    };
}