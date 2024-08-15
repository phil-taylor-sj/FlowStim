#include <Core/Domain/MeshFactory.h>

namespace fstim
{
    const Mesh* MeshFactory::operator()(Vec2i size, Vec2d length)
    {
        int nCells = size.x * size.y;
        int nFaces = (size.x + 1) * size.y + (size.y + 1) * size.x;
        Cell* cells = new Cell[size.x * size.y];
        Face* faces = new Face[nFaces];

    }

    int MeshFactory::m_calcCellId(int i, int j, Vec2i size)
    {
        return (i < 0 || i >= size.x || j < 0 || j >= size.y) 
            ? -1 
            : j * size.x + i;
    }

    void MeshFactory::m_assignIds(Cell* cells, Face* faces, Vec2i size)
    {
        for (int id = 0; id < (size.x * size.y); id++)
        {
            Cell& cell = cells[id]
            cell.neighId[Compass.NORTH] = this->m_calcCellId(cell.i, cell.j - 1, size);
            cell.neighId[Compass.SOUTH] = this->m_calcCellId(cell.i, cell.j + 1, size);
            cell.neighId[Compass.EAST] = this->m_calcCellId(cell.i + 1, cell.j, size);
            cell.neighId[Compass.WEST] = this->m_calcCellId(cell.i - 1, cell.j, size);
        
            int northStart = 2 + (size.x - 1) + (size.y - 1) * (size.x + 1);
            int northId = northStart + cell.j + cell.i * (size.y + 1);
            int southId = northId + 1;
            int westId = cell.i + cell.j * (size.x + 1);
            int eastId = westId + 1;

            faces[northId].cellId[Compass.NORTH] = id;
            faces[southId].cellId[Compass.SOUTH] = id;
            faces[eastId].cellId[Compass.EAST] = id;
            faces[westId].cellId[Compass.WEST] = id;

            cell->faceId[Compass.NORTH, northId];
            cell->faceId[Compass.SOUTH, southId];
            cell->faceId[Compass.EAST, eastId];
            cell->faceId[Compass.WEST, westId];
        }
    }

    void Mesh::m_assignFaceProperties(Cell* cells, Face* faces, Vec2i size)
    {
        for (int id = 0; id < (size.x * size.y); id++)
        {
            Cell& cell = cells[id]
            int dx = cell.width.x / 2.;
            int dy = cell.width.y / 2.;
            
            int northId = cells.faceId[Compass.NORTH];
            int southId = cells.faceId[Compass.SOUTH];
            int eastId = cells.faceId[Compass.EAST];
            int westId = cells.faceId[Compass.WEST];
            
            faces[northId].area = cell.width.x;
            faces[southId].area = cell.width.x;
            faces[eastId].area = cell.width.y;
            faces[westId].area = cell.width.y;

            faces[northId].center = cell.center - Vec2d(0., dy);
            faces[southId].center = cell.center + Vec2d(0., dy);
            faces[eastId].center = cell.center + Vec2d(dx, 0.);
            faces[westId].center = cell.center - Vec2d(dx, 0.);
        }

        int nFaces = (size.x + 1) * size.y + (size.y + 1) * size.x;
        Compass directions[] = {Compass::NORTH, Compass::SOUTH, Compass::EAST, Compass::WEST};
        for (int id = 0; id < nFaces; id++)
        { 
            // Determine if the face is connected to one cell or two.
            Face& face = faces[id];
            int total = std::count_if(directions.begin(), directions.end(), 
                [&face](Compass direction) {return face.cellId[direction] != -1;}
            );
            face.IsBoundary = (total == 1);
            
            // Calculate cell spacing.
            for (Compass direction in directions)
            {
                int cellId = face.cellId[direction];
                if (cellId == -1) { continue; } 
                face.spacing += (cells[cellId].center - face.center).mag();
            }
            if (!face.IsBoundary) {face.spacing /= 2.}
        }
    }   
}