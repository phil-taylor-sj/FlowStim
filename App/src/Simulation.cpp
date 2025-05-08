#include <Simulation.h>

Simulation::Simulation(QOpenGLWindow::UpdateBehavior updateBehavior, QWindow *parent) : QOpenGLWindow(updateBehavior, parent), m_timer(new QTimer(this))
{
    qInfo() << this << "Constructed";
    fstim::MeshFactory factory = fstim::MeshFactory();
    this->m_length = vecp::Vec2d(10., 20.);
    this->m_mesh = factory(vecp::Vec2i(10, 20), this->m_length);
    connect(m_timer, &QTimer::timeout, this, &Simulation::m_updateCanvas);
    m_timer->start(16); // 60 FPS
}

void Simulation::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color
}

void Simulation::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Simulation::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Example: Drawing three rectangles
    //for (int id = 0; id < this->m_mesh->nCells; id++)
 
    //m_drawRectangle(-0.5f, -0.5f, 0.3f, 0.3f); // Rectangle 1
    //m_drawRectangle(0.2f, 0.2f, 0.5f, 0.5f);   // Rectangle 2
    //m_drawRectangle(-0.8f, 0.2f, 0.2f, 0.4f);  // Rectangle 3
}

void Simulation::m_updateCanvas()
{
    update(); // Triggers a repaint
}

void Simulation::m_drawRectangle(const fstim::Cell2d& cell)
{
    qInfo() << "Cell Id = " << cell.id << ":" 
    << cell.vertices[0].x << cell.vertices[0].y << ",  "
    << cell.vertices[1].x << cell.vertices[1].y << ",  "
    << cell.vertices[2].x << cell.vertices[2].y << ",  "
    << cell.vertices[3].x << cell.vertices[3].y;
    glBegin(GL_LINE_LOOP);
    if (cell.id % 2 == 0)
    {
        glColor3f(1.0f, 0.0f, 0.0f); // Set rectangle color to red
    } else
    {
        glColor3f(0.0f, 1.0f, 0.0f); // Set rectangle color to red
    }
   

    for (vecp::Vec2f vertex : cell.vertices)
    {
        glVertex2f(vertex.x/20.f, vertex.y/20.f);
    }
    glEnd();
}

Simulation::~Simulation()
{
    delete this->m_timer;
    qInfo() << this << "Destructed";
}