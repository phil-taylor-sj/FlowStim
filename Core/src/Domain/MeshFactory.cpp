#include <Core/Domain/MeshFactory.h>

namespace fstim
{
    template <typename D, typename F, typename I>
    void MeshFactory<D, F, I>::m_calcCellToCellSpacing(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells) 
    {
        for (Face<D>& face : std::span<Face<D>>(faces, nFaces))
        {
            D ownerCenter = cells[face.ownerId].center;
            D neighCenter = (face.neighId < 0) ? face.center : cells[face.neighId].center;
            face.cellToCellSpacing = (ownerCenter - neighCenter).mag();
        }
    };

    template <typename D, typename F, typename I>
    void MeshFactory<D, F, I>::m_calcOwnerWeights(Face<D>* faces, int nFaces, const Cell<D, F>* cells, int nCells)
    {
        for (Face<D>& face : std::span<Face<D>>(faces, nFaces)) 
        {
            if (face.neighId < 0)
            {
                face.ownerWeight = 1.;
                continue;
            }
            double ownerDistance = (face.center - cells[face.ownerId].center).mag();
            double neighDistance = (face.center - cells[face.neighId].center).mag();
            face.ownerWeight = ownerDistance / (ownerDistance + neighDistance);
        }
    }


    template class MeshFactory<vecp::Vec2d, vecp::Vec2f, vecp::Vec2i>;
}