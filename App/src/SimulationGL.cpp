#include <SimulationGL.h>

SimulationGL::SimulationGL(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent), m_timer(new QTimer(this))
{
    qInfo() << this << "Constructed";
    fstim::MeshFactory factory = fstim::MeshFactory();
    this->m_length = vecp::Vec2d(10., 20.);
    this->m_mesh = factory(vecp::Vec2i(10, 20), this->m_length);
    connect(m_timer, &QTimer::timeout, this, &SimulationGL::m_updateCanvas);
    m_timer->start(16); // 60 FPS
}

void SimulationGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color
}

void SimulationGL::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SimulationGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (int id = 0; id < this->m_mesh->nCells; id++)
    {
        m_drawRectangle(this->m_mesh->cells[id]);
    }
}

void SimulationGL::m_updateCanvas()
{
    update(); // Triggers a repaint
}

void SimulationGL::m_drawRectangle(const fstim::Cell& cell)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
   
    for (vecp::Vec2f vertex : cell.verticies)
    {
        glVertex2f((vertex.x - 5.f) / 12.f, (vertex.y - 10.f)/12.f);
    }
    glEnd();
}

SimulationGL::~SimulationGL()
{
    delete this->m_timer;
    qInfo() << this << "Destructed";
}