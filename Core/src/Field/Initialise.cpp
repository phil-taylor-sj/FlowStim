#include <Core/Field/Initialise.h>

namespace fstim
{
    template<typename T>
    void Initialise<T>::rectangle(
        const Mesh& mesh, Field<T>& field, vecp::Vec2i center, vecp::Vec2i width
        )
    {
        for (size_t id = 0; id < mesh.nCells; id++)
        {
            const Cell& cell = mesh.cells[id];
        }
    }
}