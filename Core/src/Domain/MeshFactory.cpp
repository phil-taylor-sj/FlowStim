#include <Core/Domain/MeshFactory.h>

namespace fstim
{
    std::unique_ptr<Mesh> MeshFactory::operator()(Vec2i size, Vec2d length)
    {
        int nCells = size.x * size.y;
        int nFaces = (size.x + 1) * size.y + (size.y + 1) * size.x;
        
        std::unique_ptr<Cell[]> cells(new Cell[nCells]);
        std::unique_ptr<Face[]> faces(new Face[nFaces]);

        // Configure all cells, faces, and vertices.
        this->m_assignIds(cells, faces, size);
        this->m_assignProperties(cells, faces, size, length);
        this->m_assignVertices(cells, faces, size);

        // Filter all vertices to keep onyl unique vertices.
        std::vector<Vertex> verticesArr = VertexMapping::createVertices(cells.get(), nCells, faces.get(), nFaces);
        int nVertices = verticesArr.size();
        
        // Verify all unique vertices are included exactly once.
        int expected_vertices = (size.x + 1) * (size.y + 1);
        if (nVertices != expected_vertices)
        {
            std::string msg = std::format(
                "Mesh Validation Failed: Number of unique vertices ({}) not equal to expected value ({}).",
                nVertices, expected_vertices);
            //throw std::runtime_error(msg);
        }
        
        // Transfer unique vertices into a new C-style array.
        std::unique_ptr<Vertex[]> vertices(new Vertex[nVertices]);
        for (Vertex vertex : verticesArr)
        {
            vertices[vertex.vertexId] = vertex;
        }

        return std::move(
            std::make_unique<Mesh>(nCells, nFaces, nVertices, std::move(cells), std::move(faces), std::move(vertices), length)
        );
    }

    Vec2i MeshFactory::m_getCellLocations(int id, Vec2i size)
    {
        int j = id / size.x;
        int i = id - j * size.x;
        return Vec2i(i, j);
    }

    int MeshFactory::m_calcCellId(int i, int j, Vec2i size)
    {
        int nCells = size.x * size.y;
        return (i < 0 || i >= size.x || j < 0 || j >= size.y)
            ? -1
            : j * size.x + i;
    }

    void MeshFactory::m_assignIds(std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, Vec2i size)
    { 

        int northStart = 2 + (size.x - 1) + (size.y - 1) * (size.x + 1);
        for (int i = 0; i < size.x; i++)
        {
            for (int j = 0; j < size.y; j++)
            {
                int cellId = this->m_calcCellId(i, j, size);
                Cell& cell = cells[cellId];
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
                    Face& face = faces[faceIds[direction]];
                    if (face.ownerId == -1) {face.ownerId = cell.id;}
                    else {face.neighId = cell.id;};
                    cell.faceId[(int)direction] = faceIds[direction];
                }
            }
        }
        
    }

    void MeshFactory::m_assignProperties(std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, Vec2i size, Vec2d length)
    {
        int nCells = size.x * size.y;
        std::tuple<std::vector<double>, std::vector<double>> gridX = (*this->m_profile)(size.x, length.x);
        std::tuple<std::vector<double>, std::vector<double>> gridY = (*this->m_profile)(size.y, length.y);

        for (int id = 0; id < (nCells); id++)
        {
            Vec2i locations = this->m_getCellLocations(id, size);
            int i = locations.x;
            int j = locations.y;

            Cell& cell = cells[id];
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
                
                Face& face = faces[faceId];
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
            Face& face = faces[id];
            face.id = id;      
            Cell owner = cells[face.ownerId]; 
            //face.spacing = (face.neighId >= 0)
            //    ? (cells[face.neighId].center - owner.center).mag()
            //    : (face.center - owner.center).mag();
        }
    }

    void MeshFactory::m_assignVertices(
        std::unique_ptr<Cell[]>& cells, std::unique_ptr<Face[]>& faces, vecp::Vec2i size
    )
    {
        int nCells = size.x * size.y;
        for (int id = 0; id < nCells; id++)
        {
            Cell& cell = cells[id];
            for (int sideId = 0; sideId < cell.faceId.size(); sideId++)
            {
                Face& face = faces[cell.faceId[sideId]];
                double angle = (face.ownerId == cell.id) ? -90. : 90.;
                vecp::Vec2d tangent = (face.normal.rotate(angle)) * 0.5;
                cell.vertices[sideId] = (face.center + tangent).toFloat(); 
            }
        }
    }
}