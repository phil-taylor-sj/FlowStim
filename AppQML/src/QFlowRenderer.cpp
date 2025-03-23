#include <AppQML/QFlowRenderer.h>

QFlowRenderer::QFlowRenderer()
{
    glClearColor(1.f, 1.f, 1.f, 0.f);
    this->m_createBuffer(this->m_cellBuffers);
    this->m_createBuffer(this->m_gridBuffers);
    this->m_createShader();
}

QFlowRenderer::m_createBuffer(QBuffers& buffers)
{
    buffers.vertexArray.create();
    buffers.vertexArray.bind();

    buffers.vertexBuffer.create();
    buffers.vertexBuffer.bind();
    buffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    buffers.colourBuffer.create();
    buffers.colourBuffer.bind();
    buffers.colourBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    buffers.indexBuffer.create();
    buffers.vertexArray.release();
}

void QFlowRenderer::m_createShader()
{
    m_cellBuffers.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexTest.shader");
    m_cellBuffers.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.shader");
    m_cellBuffers.shader.link();

    m_gridBuffers.shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexTest.shader");
    m_gridBuffers.shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.shader");
    m_gridBuffers.shader.link();
}
void QFlowRenderer::m_createCellMesh(std::shared_ptr<MeshData>& data)
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

    this->cellBuffers.vertexArray.bind();
    // Continue from here.
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