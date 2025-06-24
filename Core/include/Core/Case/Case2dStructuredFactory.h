#pragma once


#include <Core/Case/CaseFactory.h>
#include <Core/Domain/Mesh.h>

#include <VecPlus/Vec2.h>

#include <memory>

namespace fstim
{

    class Case2dStructuredFactory : public CaseFactory<vecp::Vec2d>
    {
    public:
        void setDomainSize(vecp::Vec2i size);

        vecp::Vec2i getDomainSize();

        void setDomainLength(vecp::Vec2d length);

        vecp::Vec2d getDomainLength();

        Case2dStructuredFactory() {};

        virtual ~Case2dStructuredFactory() {}; 
    
    protected:
        const vecp::Vec2i m_minimumDomainSize {4, 4};
        const vecp::Vec2d m_minimumDomainLength {1e-06, 1e-06};

        const vecp::Vec2i m_defaultDomainSize {5, 5};
        const vecp::Vec2d m_defaultDomainLength {1., 1.};

        vecp::Vec2i m_domainSize = m_defaultDomainSize;
        vecp::Vec2d m_domainLength = m_defaultDomainLength;

        int m_nCells = 16;

        std::unique_ptr<Mesh2d> m_createMesh();

        bool m_isReferenceDirectionValid(Compass direction) override;

    };
}