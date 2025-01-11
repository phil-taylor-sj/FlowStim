#include <SimulationGL.h>


SimulationGL::SimulationGL(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent), m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &SimulationGL::m_updateCanvas);
    m_timer->start(32); // 60 FPS
}

void SimulationGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.f, 1.f, 1.f, 0.f);

    this->m_cellVao.create();
    this->m_cellVao.bind();

    this->m_cellVertexBuffer.create();
    this->m_cellVertexBuffer.bind();
    this->m_cellVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    this->m_cellColourBuffer.create();
    this->m_cellColourBuffer.bind();
    this->m_cellColourBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    this->m_cellIndexBuffer.create();
    this->m_cellVao.release();

    this->m_gridVao.create();
    this->m_gridVao.bind();

    this->m_gridVertexBuffer.create();
    this->m_gridVertexBuffer.bind();
    this->m_gridVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    this->m_gridColourBuffer.create();
    this->m_gridColourBuffer.bind();
    this->m_gridColourBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    
    this->m_gridIndexBuffer.create();
    this->m_gridVao.release();
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

    this->m_createCellMesh(data);
    this->m_createGridMesh(data);

    this->m_velocity = nullptr;

    //MeshGL::createVertexArray(this->m_vaoMesh, this->m_vbaMesh, *data.get());
    //MeshGL::createElementArray(this->m_vaoMesh, this->m_iboMesh, *data.get());
    this->m_createShader();
}

void SimulationGL::recieveVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data)
{
    std::lock_guard<std::mutex> guard(this->m_mutex);
    this->m_velocity = data;
    //this->m_cellVao.bind();
    //this->m_cellColourBuffer.bind();

    this->m_gridVao.bind();
    this->m_cellColourBuffer.bind();

    //unsigned int total = this->m_nCells * 4;
    unsigned int total = this->m_nVertices;
    
    /**
    std::vector<float> colours(total * 3);
    for (unsigned int vId = 0; vId < total; vId++)
    {
        colours[vId * 3] = 0.f;
        //colours[vId * 3 + 1] = (vId >= total / 2) ? 0.f : 1.f;
        //colours[vId * 3 + 2] = (vId >= total / 2) ? 1.f : 0.f;
        float velocity = (*m_velocity)[vId].mag();
        if (velocity >= 0.f)
        {
            float value = std::min(1.f * abs(velocity) / 1.f, 1.f);
            colours[vId * 3 + 1] = 1.f - value;
            colours[vId * 3 + 2] = value; 
        }
        else
        {
            // green velocity < 0;
            colours[vId * 3 + 2] = 0.f;
            colours[vId * 3 + 1] = 1.f * abs(velocity) / 1.f; 
        }
    }
    m_gridColourBuffer.allocate(colours.data(), colours.size() * sizeof(float));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    */

    std::vector<float> values(this->m_nVertices);
    for (unsigned int vId = 0; vId < total; vId++)
    {
        //values[vId] = (*m_velocity)[vId].mag();
        values[vId] = (float)vId;
    }

    m_gridColourBuffer.allocate(values.data(), values.size() * sizeof(float));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    //m_cellColourBuffer.allocate(colours.data(), colours.size() * sizeof(float));
    this->m_gridVao.release();
}

void SimulationGL::m_updateCanvas()
{
    update();
}

void SimulationGL::m_drawField()
{
    this->m_shader->bind();
    this->m_gridVao.bind();
    int loc_maxValue = m_shader->uniformLocation("u_maxValue");
    assert(loc_maxValue != -1);
    m_shader->setUniformValue(loc_maxValue, 200.f);
    glDrawElements(GL_TRIANGLES, this->m_nCells * 6, GL_UNSIGNED_INT, nullptr);
    this->m_gridVao.release();
}

void SimulationGL::m_drawMesh()
{   
    this->m_shader->bind();
    this->m_cellVao.bind();
    for (int index = 0; index < this->m_nCells; index++)
    {
        int start = index * 4;
        glDrawArrays(GL_LINE_LOOP, start, 4);
    }
    this->m_cellVao.release();
}

SimulationGL::~SimulationGL()
{
    delete this->m_timer;

    this->m_deleteBuffers();
}

void SimulationGL::m_createCellMesh(std::shared_ptr<MeshData>& data)
{
    std::vector<float> allVertices(2 * 4 * data->nCells);
    size_t count = 0;
    for (std::vector<size_t> vertexIds : data->cellElements)
    {
        for (size_t vertexId : vertexIds)
        {
            vecp::Vec2f position = data->vertices[vertexId];
            allVertices[count++] = position.x;
            allVertices[count++] = position.y;
        }
    }

    this->m_cellVao.bind();
    this->m_cellVertexBuffer.bind();

    this->m_cellVertexBuffer.allocate(allVertices.data(), allVertices.size() * sizeof(float));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    this->m_nCells = data->nCells;
    this->m_cellIndexBuffer.bind();
    std::vector<unsigned int> indices(data->nCells * 6);
    const std::array<unsigned int, 6> index = {0, 1, 3, 1, 2, 3};
    for (int cellId = 0; cellId < data->nCells; cellId++)
    {
        for (unsigned int shift = 0; shift < 6; shift++)
        {
            indices[(cellId * 6) + shift] = (cellId * 4) + index[shift];
        }
    }
    this->m_cellIndexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    this->m_cellVao.release();

    this->m_velocity = nullptr;
}


void SimulationGL::m_createGridMesh(std::shared_ptr<MeshData>& data)
{
    std::vector<float> vertices(2 * data->vertices.size());
    size_t count = 0;
    for (vecp::Vec2f position : data->vertices)
    {
            vertices[count++] = position.x;
            vertices[count++] = position.y;
    }

    this->m_gridVao.bind();
    this->m_gridVertexBuffer.bind();

    this->m_gridVertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(float));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    this->m_nVertices = data->vertices.size();
    this->m_gridIndexBuffer.bind();
    std::vector<unsigned int> indices(data->nCells * 6);
    const std::array<unsigned int, 6> index = {0, 1, 3, 1, 2, 3};

    count = 0;
    for (std::vector<size_t> cell : data->cellElements)
    {
        for (unsigned int loc = 0; loc < index.size(); loc++)
        {
            indices[count++] = cell[index[loc]];
        }
    }
    this->m_gridIndexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    this->m_gridVao.release();
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
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexTest.shader");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.shader");
    m_shader->link();

    //this->m_shader = ShaderGL::createShader(vertexShader, fragmentShader); 
    //glUseProgram(this->m_shader);

}

void SimulationGL::m_deleteBuffers()
{
    //glDeleteBuffers(1, &m_vbaMesh);
    //glDeleteBuffers(1, &m_vaoMesh);
    //glDeleteBuffers(1, &m_iboMesh);
}