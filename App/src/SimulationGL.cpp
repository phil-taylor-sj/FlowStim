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

    this->m_vao.create();
    this->m_vao.bind();

    this->m_vertexBuffer.create();
    this->m_vertexBuffer.bind();
    this->m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    this->m_colourBuffer.create();
    this->m_colourBuffer.bind();
    this->m_colourBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);


    this->m_indexBuffer.create();
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

    this->m_drawMesh();
}

void SimulationGL::recieveMesh(std::shared_ptr<MeshData> data)
{   
    std::lock_guard<std::mutex> guard(this->m_mutex);
    
    this->m_vao.bind();
    this->m_vertexBuffer.bind();
    
    this->m_vertexBuffer.allocate(data->vertices.data(), data->vertices.size() * sizeof(float));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    this->m_nCells = data->nCells;
    this->m_indexBuffer.bind();
    std::vector<unsigned int> indices(data->nCells * 6);
    const std::array<unsigned int, 6> index = {0, 1, 3, 1, 2, 3};
    for (int cellId = 0; cellId < data->nCells; cellId++)
    {
        for (unsigned int shift = 0; shift < 6; shift++)
        {
            indices[(cellId * 6) + shift] = (cellId * 4) + index[shift];
        }
    }
    this->m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    this->m_vao.release();

    this->m_velocity = nullptr;

    //MeshGL::createVertexArray(this->m_vaoMesh, this->m_vbaMesh, *data.get());
    //MeshGL::createElementArray(this->m_vaoMesh, this->m_iboMesh, *data.get());
    this->m_createShader();
}

void SimulationGL::recieveVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data)
{
    std::lock_guard<std::mutex> guard(this->m_mutex);
    this->m_velocity = data;

    this->m_vao.bind();
    this->m_colourBuffer.bind();
    
    unsigned int total = this->m_nCells * 4;
    std::vector<float> colours(total * 3);
    for (unsigned int vId = 0; vId < this->m_nCells * 4; vId++)
    {
        colours[vId * 3] = 0.f;
        colours[vId * 3 + 1] = (vId >= total / 2) ? 0.f : 1.f;
        colours[vId * 3 + 2] = (vId >= total / 2) ? 1.f : 0.f;
    }

    m_colourBuffer.allocate(colours.data(), colours.size() * sizeof(float));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    this->m_vao.release();
   
}

void SimulationGL::m_updateCanvas()
{
    update();
}

void SimulationGL::m_drawField()
{
    this->m_shader->bind();
    this->m_vao.bind();
    glDrawElements(GL_TRIANGLES, this->m_nCells * 6, GL_UNSIGNED_INT, nullptr);
    this->m_vao.release();
}

void SimulationGL::m_drawMesh()
{   
    this->m_shader->bind();
    this->m_vao.bind();
    for (int index = 0; index < this->m_nCells; index++)
    {
        int start = index * 4;
        glDrawArrays(GL_LINE_LOOP, start, 4);
    }
    this->m_vao.release();
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
    //this->m_fragmentShader->compileSourceCode(QString::fromStdString(fragmentShader));
    //this->m_shader->addShader(this->m_vertexShader.get());
    //this->m_shader->addShader(this->m_fragmentShader.get());
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.shader");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.shader");
    //this->m_shader = ShaderGL::createShader(vertexShader, fragmentShader); 
    //glUseProgram(this->m_shader);

}

void SimulationGL::m_deleteBuffers()
{
    //glDeleteBuffers(1, &m_vbaMesh);
    //glDeleteBuffers(1, &m_vaoMesh);
    //glDeleteBuffers(1, &m_iboMesh);
}