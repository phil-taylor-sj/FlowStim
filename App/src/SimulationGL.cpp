#include <SimulationGL.h>


SimulationGL::SimulationGL(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent), m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &SimulationGL::m_updateCanvas);
    m_timer->start(32); // 60 FPS
}

void SimulationGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.f, 0.f, 0.f, 1.f);
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
        //this->m_drawField(); 
    }

    this->m_drawMesh();
}

void SimulationGL::recieveMesh(std::shared_ptr<MeshData> data)
{   
    std::lock_guard<std::mutex> guard(this->m_mutex);
    
    this->m_deleteBuffers();
    this->m_velocity = nullptr;
    this->m_nCells = data->nCells;
    MeshGL::createVertexArray(this->m_vaoMesh, this->m_vbaMesh, *data.get());
    MeshGL::createElementArray(this->m_vaoMesh, this->m_iboMesh, *data.get());
    this->m_createShader();
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
    glBindVertexArray(m_vaoMesh);
    glDrawElements(GL_TRIANGLES, this->m_nCells * 3 + 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SimulationGL::m_drawMesh()
{   
    glClear(GL_COLOR_BUFFER_BIT);
    this->m_shader->bind();
    glBindVertexArray(m_vaoMesh);
    for (int index = 0; index < this->m_nCells; index++)
    {
        int start = index * 4;
        glDrawArrays(GL_LINE_LOOP, start, 4);
    }
    glBindVertexArray(0);
}

SimulationGL::~SimulationGL()
{
    delete this->m_timer;

    this->m_deleteBuffers();
}

void SimulationGL::m_createShader()
{
    std::string vertexShader = 
        "#version 410 core\n"
        "\n"
        "in vec2 position;"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position, 0.0, 1.0);\n"
        "}\n";

    std::string fragmentShader = 
        "#version 410 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n";

    this->m_shader = std::make_unique<QOpenGLShaderProgram>();
    
    this->m_vertexShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Vertex);
    this->m_vertexShader->compileSourceCode(QString::fromStdString(vertexShader));
    
    this->m_fragmentShader = std::make_unique<QOpenGLShader>(QOpenGLShader::Fragment);
    this->m_fragmentShader->compileSourceCode(QString::fromStdString(fragmentShader));
   
    this->m_shader->addShader(this->m_vertexShader.get());
    this->m_shader->addShader(this->m_fragmentShader.get());

    //this->m_shader = ShaderGL::createShader(vertexShader, fragmentShader); 
    //glUseProgram(this->m_shader);

}

void SimulationGL::m_deleteBuffers()
{
    glDeleteBuffers(1, &m_vbaMesh);
    glDeleteBuffers(1, &m_vaoMesh);
    glDeleteBuffers(1, &m_iboMesh);
}