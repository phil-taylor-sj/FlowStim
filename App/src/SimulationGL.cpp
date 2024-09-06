#include <SimulationGL.h>


SimulationGL::SimulationGL(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent), m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &SimulationGL::m_updateCanvas);
    m_timer->start(16); // 60 FPS
}

void SimulationGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SimulationGL::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SimulationGL::paintGL()
{
   
    std::lock_guard<std::mutex> guard(this->renderMutex);
    if (this->m_isMeshLoaded)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        m_drawRectangle();
    }
}

void SimulationGL::loadMesh(const fstim::Mesh* mesh)
{    
    this->m_length = mesh->length;
    this->m_nCells = mesh->nCells;

    std::vector<float> points(2 * 4 * this->m_nCells);
    const fstim::Cell* begin = mesh->cells.get(); 
    const fstim::Cell* end = begin + mesh->nCells;

    int index = 0;
    for (const fstim::Cell* cell = begin; cell != end; cell++)
    {
        for (vecp::Vec2f vertex : cell->vertices)
        {
            points[index++] = (vertex.x - 5.f) / 12.f;
            points[index++] = (vertex.y - 10.f) / 12.f;
        }
    }

    // for (int j = 0; j < index; j+=2)
    // {
    //     std::cout << "[" << points[j] << ", " << points[j+1] << "]   ";
    // }
    // std::cout << std::endl;

    std::lock_guard<std::mutex> guard(this->renderMutex);
    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, (index) * sizeof(float), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->m_isMeshLoaded = true;
}


void SimulationGL::m_updateCanvas()
{
        update();
}


void SimulationGL::m_drawRectangle()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer);  // Bind buffer for drawing
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    int nPoints  = 2 * 4 * this->m_nCells;
    for (int index = 0; index < this->m_nCells; index++)
    {
        int start = index * 4;
        glDrawArrays(GL_LINE_LOOP, start, 4);
    }

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SimulationGL::~SimulationGL()
{
    delete this->m_timer;
}