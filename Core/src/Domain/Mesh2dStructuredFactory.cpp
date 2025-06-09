#include <Core/Domain/Mesh2dStructuredFactory.h>

namespace fstim
{
    std::unique_ptr<Mesh2d> Mesh2dStructuredFactory::operator()(vecp::Vec2i size, vecp::Vec2d length)
    {
        m_nCells = size.x * size.y;
        m_nFaces = (size.x + 1) * size.y + (size.y + 1) * size.x;

        m_cells = std::make_unique<Cell2d[]>(m_nCells);
        m_faces = std::make_unique<Face2d[]>(m_nFaces);

        // Configure all cells, faces, and vertices.
        this->m_assignIds(size);
        this->m_assignProperties(size, length);
        this->m_assignVertices(size);

        // Filter all vertices to keep onyl unique vertices.
        std::vector<Vertex2d> verticesArr = VertexMapping::createVertices(m_cells.get(), m_nCells, m_faces.get(), m_nFaces);
        m_nVertices = verticesArr.size();
        
        // Verify all unique vertices are included exactly once.
        int expected_vertices = (size.x + 1) * (size.y + 1);
        if (this->m_nVertices != expected_vertices)
        {
            std::string msg = std::format(
                "Mesh Validation Failed: Number of unique vertices ({}) not equal to expected value ({}).",
                this->m_nVertices, expected_vertices);
            //throw std::runtime_error(msg);
        }
        
        // Transfer unique vertices into a new C-style array.
        this->m_vertices = std::make_unique<Vertex2d[]>(this->m_nVertices);
        for (Vertex2d vertex : verticesArr)
        {
            this->m_vertices[vertex.vertexId] = vertex;
        }

        m_calcCellToCellSpacing(m_faces.get(), m_nFaces, m_cells.get(), m_nCells);
        m_calcOwnerWeights(m_faces.get(), m_nFaces, m_cells.get(), m_nCells);

        std::unique_ptr<Mesh2d> mesh = this->m_createMesh(length);

        return std::move(mesh);
    }

    vecp::Vec2i Mesh2dStructuredFactory::m_getCellLocations(int id, vecp::Vec2i size)
    {
        int j = id / size.x;
        int i = id - j * size.x;
        return vecp::Vec2i(i, j);
    }

    int Mesh2dStructuredFactory::m_calcCellId(int i, int j, vecp::Vec2i size)
    {
        int nCells = size.x * size.y;
        return (i < 0 || i >= size.x || j < 0 || j >= size.y)
            ? -1
            : j * size.x + i;
    }

    void Mesh2dStructuredFactory::m_assignIds(vecp::Vec2i size)
    { 

        int northStart = 2 + (size.x - 1) + (size.y - 1) * (size.x + 1);
        for (int i = 0; i < size.x; i++)
        {
            for (int j = 0; j < size.y; j++)
            {
                int cellId = this->m_calcCellId(i, j, size);
                Cell2d& cell = this->m_cells[cellId];
                cell.id = cellId;

                std::map<Compass, int> faceIds = {
                    {Compass::NORTH, northStart + i + (j + 1) * (size.x)},
                    {Compass::SOUTH, northStart + i + j * (size.x)},
                    {Compass::EAST, i + j * (size.x + 1) + 1}, // eastId + 1
                    {Compass::WEST, i + j * (size.x + 1)}
                };
            
                cell.faceId.resize(4);
                cell.vertices.resize(4);
                for (Compass direction : this->m_directions)
                {
                    Face2d& face = m_faces[faceIds[direction]];
                    if (face.ownerId == -1) {face.ownerId = cell.id;}
                    else {face.neighId = cell.id;};
                    cell.faceId[(int)direction] = faceIds[direction];
                }
            }
        }
        
    }

    void Mesh2dStructuredFactory::m_assignProperties(vecp::Vec2i size, vecp::Vec2d length)
    {
        int nCells = size.x * size.y;
        std::tuple<std::vector<double>, std::vector<double>> gridX = (*this->m_profile)(size.x, length.x);
        std::tuple<std::vector<double>, std::vector<double>> gridY = (*this->m_profile)(size.y, length.y);

        for (int id = 0; id < (nCells); id++)
        {
            vecp::Vec2i locations = this->m_getCellLocations(id, size);
            int i = locations.x;
            int j = locations.y;

            Cell2d& cell = this->m_cells[id];
            double dx = std::get<1>(gridX)[i];
            double dy = std::get<1>(gridY)[j];
            cell.volume = dx * dy;

            cell.center.x = std::get<0>(gridX)[i];
            cell.center.y = std::get<0>(gridY)[j];

            std::map<Compass, std::tuple<double, vecp::Vec2d>> properties = {
                {Compass::NORTH, std::make_tuple(dx, vecp::Vec2d(0., 0.5 * dy))},
                {Compass::SOUTH, std::make_tuple(dx, vecp::Vec2d(0., 0.5 * -dy))},
                {Compass::EAST, std::make_tuple(dy, vecp::Vec2d(0.5 * dx, 0.))},
                {Compass::WEST, std::make_tuple(dy, vecp::Vec2d(0.5 * -dx, 0.))}
            };

            for (Compass direction : this->m_directions)
            {
                int faceId = cell.faceId[(int)direction];
                double area = std::get<0>(properties[direction]);
                
                Face2d& face = this->m_faces[faceId];
                face.center = cell.center + std::get<1>(properties[direction]);
                if (face.ownerId == cell.id) 
                {
                    face.normal = (face.center - cell.center).normalise() * area;
                }
            }
        }

        int nFaces = (size.x + 1) * size.y + (size.y + 1) * size.x;
        for (int id = 0; id < nFaces; id++)
        {       
            Face2d& face = this->m_faces[id];
            face.id = id;      
            Cell2d owner = this->m_cells[face.ownerId]; 
            //face.spacing = (face.neighId >= 0)
            //    ? (cells[face.neighId].center - owner.center).mag()
            //    : (face.center - owner.center).mag();
        }
    }

    void Mesh2dStructuredFactory::m_assignVertices(vecp::Vec2i size)
    {
        for (int id = 0; id < this->m_nCells; id++)
        {
            Cell2d& cell = this->m_cells[id];
            for (int sideId = 0; sideId < cell.faceId.size(); sideId++)
            {
                Face2d& face = this->m_faces[cell.faceId[sideId]];
                double angle = (face.ownerId == cell.id) ? -90. : 90.;
                vecp::Vec2d tangent = (face.normal.rotate(angle)) * 0.5;
                cell.vertices[sideId] = (face.center + tangent).toFloat(); 
            }
        }
    }
}