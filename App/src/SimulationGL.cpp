#include <SimulationGL.h>


SimulationGL::SimulationGL(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent), m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &SimulationGL::m_updateCanvas);
    m_timer->start(32); // 60 FPS
}

void SimulationGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.f, 1.0f);
}

void SimulationGL::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SimulationGL::paintGL()
{

    std::lock_guard<std::mutex> guard(this->m_mutex);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (this->m_nCells == 0) 
    { 
        return; 
    }
    
    if (this->m_velocity != nullptr) 
    {
        this->m_drawField(); 
    }
    else
    {
        qDebug() << "Velocity is nullPtr";
    }

    this->m_drawMesh();
}

void SimulationGL::recieveMesh(std::shared_ptr<MeshData> data)
{   
    std::lock_guard<std::mutex> guard(this->m_mutex);

    this->m_deleteBuffers();

    m_velocity = nullptr;

    MeshGL::createVertexArray(this->m_vaoMesh, this->m_vbaMesh, *data.get());

    this->m_nCells = data->nCells;

    std::vector<unsigned int> indices(this->m_nCells * 6);
    const std::array<unsigned int, 6> index = {0, 1, 3, 1, 2, 3};
    for (int cellId = 0; cellId < this->m_nCells; cellId++)
    {
        unsigned int startId = cellId * 6;
        unsigned int startIndex = cellId * 4;
        
        for (unsigned int shift = 0; shift < 6; shift++)
        {
            indices[(cellId * 6) + shift] = (cellId * 4) + index[shift];
        }
    }

    glGenBuffers(1, &this->m_iboMesh);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_iboMesh);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void SimulationGL::recieveVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data)
{
    std::lock_guard<std::mutex> guard(this->m_mutex);
    this->m_velocity = data;
}

void SimulationGL::m_updateCanvas()
{
    update();
}

void SimulationGL::m_drawField()
{
    std::vector<float> colours;
    colours.resize(this->m_nCells * 4 * 3);   

    std::vector<vecp::Vec2f>& velocity = *(this->m_velocity);
    for (int cellId = 0; cellId < this->m_nCells; ++cellId)
    {
        // Random RGB color for the current quad
        float red = 0.5f;
        float green = 0.5f;
        float blue = 0.5f;
        //float blue = std::min(velocity[cellId].x, 1.f);

        // Set the same color for all 4 vertices of the quad
        for (int index = 0; index < 4; ++index)
        {
            colours[(cellId * 4 + index) * 3 + 0] = red; // Red
            colours[(cellId * 4 + index) * 3 + 1] = green; // Green
            colours[(cellId * 4 + index) * 3 + 2] = blue; // Blue
        }
    }

    //glGenBuffers(1, &m_fieldBuffer); // Color buffer
    //glBindBuffer(GL_ARRAY_BUFFER, m_fieldBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colours.size(), colours.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(m_vaoMesh);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_iboMesh);
    glEnableVertexAttribArray(0);    

    glDrawElements(GL_TRIANGLES, this->m_nCells * 3 + 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimulationGL::m_drawMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbaMesh);  // Bind buffer for drawing
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

    this->m_deleteBuffers();
}

void SimulationGL::m_deleteBuffers()
{
    glDeleteBuffers(1, &m_vbaMesh);
     glDeleteBuffers(1, &m_vaoMesh);
    glDeleteBuffers(1, &m_fieldBuffer);
    glDeleteBuffers(1, &m_iboMesh);
    glDeleteVertexArrays(1, &m_vertexArrayBuffer);
}