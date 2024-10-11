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
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (this->m_nCells == 0) 
    { 
        return; 
    }
    
    if (this->m_velocity != nullptr) 
    { 
        m_drawField(); 
    }

    m_drawMesh();
}

void SimulationGL::recieveMesh(std::shared_ptr<MeshData> data)
{   
    std::lock_guard<std::mutex> guard(this->renderMutex);

    m_velocity = nullptr;

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(
    GL_ARRAY_BUFFER, 
        data->vertices.size() * sizeof(float), 
        data->vertices.data(), GL_STATIC_DRAW
        );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->m_nCells = data->nCells;
}

void SimulationGL::recieveVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data)
{
    std::lock_guard<std::mutex> guard(this->renderMutex);
    this->m_velocity = data;
}

void SimulationGL::m_updateCanvas()
{
    update();
}

void SimulationGL::m_drawField()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer);  // Bind buffer for drawing
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    std::vector<vecp::Vec2f>& velocity = *m_velocity;
    for (int cellId = 0; cellId < this->m_nCells; cellId++)
    {
        float value = std::min(velocity[cellId].x, 1.f);
        glColor3f(value, 0.0f, 0.0f);
        int start = cellId * 4;
        glDrawArrays(GL_QUADS, start, 4);
    }

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimulationGL::m_drawMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer);  // Bind buffer for drawing
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    for (int cellId = 0; cellId < this->m_nCells; cellId++)
    {
        int start = cellId * 4;
        glDrawArrays(GL_LINE_LOOP, start, 4);
    }

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SimulationGL::~SimulationGL()
{
    delete this->m_timer;
}