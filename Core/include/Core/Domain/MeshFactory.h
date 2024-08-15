#pragma once 

#include <VecPlus/Vec2.h>

#include <Core/Domain/Cell.h>
#include <Core/Domain/Face.h>
#include <Core/Domain/Mesh.h>
#include <Core/Domain/Compass.h>

using namespace vecp;

namespace fstim
{
    class MeshFactory
    {
    public:
        const Mesh* operator()(Vec2i size, Vec2d length);
        
    private:
        int m_calcCellId(int i, int j, Vec2i size);
        
        void m_assignIds(Cell* cells, Face* faces, Vec2d length);
    
    }
    
}