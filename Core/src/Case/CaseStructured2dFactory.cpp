#include <Core/Case/CaseStructured2dFactory.h>

#include <Core/Domain/Mesh2dStructuredFactory.h>

#include <VecPlus/Vec2.h>
#include <format>
#include <iostream>
#include <string>


namespace fstim
{
    void CaseStructured2dFactory::setDomainSize(vecp::Vec2i size)
    {
        std::string message = "Component {0} of domain size cannot be less than {1}:"
            "Setting domain size to default values.";
        if (size.x < m_minimumDomainSize.x)
        {
            std::cout << std::vformat(message, std::make_format_args("x", m_minimumDomainSize.x)) << std::endl;
            m_domainSize = m_defaultDomainSize;
        }
        else if (size.y < m_minimumDomainSize.y)
        {
            std::cout << std::vformat(message, std::make_format_args("y", m_minimumDomainSize.y)) << std::endl;
            m_domainSize = m_defaultDomainSize;
        }
        else
        {
            m_domainSize = size;
        }
    }

    vecp::Vec2i CaseStructured2dFactory::getDomainSize()
    {
        return m_domainSize;
    }

    void CaseStructured2dFactory::setDomainLength(vecp::Vec2d length)
    {        
        std::string message = "Component {0} of domain length cannot be less than {1}:"
            "Setting domain length to default values.";
        if (length.x < m_minimumDomainLength.x)
        {
            std::cout << std::vformat(message, std::make_format_args("x", m_minimumDomainLength.x)) << std::endl;
            m_domainLength = m_defaultDomainLength;
        }
        else if (length.y < m_minimumDomainLength.y)
        {
            std::cout << std::vformat(message, std::make_format_args("y", m_minimumDomainLength.y)) << std::endl;
            m_domainLength = m_defaultDomainLength;
        }
        else
        {
            m_domainLength = length;
        }
    }

    vecp::Vec2d CaseStructured2dFactory::getDomainLength()
    {
        return m_domainLength;
    }

    std::unique_ptr<Mesh2d> CaseStructured2dFactory::m_createMesh()
    {
        Mesh2dStructuredFactory factory;
        return std::move(factory(m_domainSize, m_domainLength));
    }
}