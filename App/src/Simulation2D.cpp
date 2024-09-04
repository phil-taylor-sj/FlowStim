#include <Simulation2D.h>


void Simulation2D::start()
{
    emit this->renderMesh(this->m_mesh.get());
}

Simulation2D::Simulation2D(QObject *parent) : QObject(parent), m_timer(new QTimer(this))
{
    fstim::MeshFactory factory = fstim::MeshFactory();
    this->m_length = vecp::Vec2d(10., 20.);
    this->m_mesh = factory(vecp::Vec2i(10, 10), this->m_length);

    //qInfo() << this << "Constructed";
}

void Simulation2D::m_sendMesh()
{
    
}

Simulation2D::~Simulation2D()
{
    delete this->m_timer;
    //qInfo() << this << "Destructed";
}