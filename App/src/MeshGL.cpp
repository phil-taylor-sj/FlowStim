#include <MeshGL.h>

void MeshGL::createVertexArray(unsigned int& vao, unsigned int& vbo, MeshData& data)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    unsigned int size = data.vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size, data.vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshGL::createElementArray(unsigned int vao, unsigned int& ibo, MeshData& data)
{
    std::vector<unsigned int> indices(data.nCells * 6);
    const std::array<unsigned int, 6> index = {0, 1, 3, 1, 2, 3};
    for (int cellId = 0; cellId < data.nCells; cellId++)
    {
        //unsigned int startId = cellId * 6;
        //unsigned int startIndex = cellId * 4;
        for (unsigned int shift = 0; shift < 6; shift++)
        {
            indices[(cellId * 6) + shift] = (cellId * 4) + index[shift];
        }
    }

    glBindVertexArray(vao);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);


/**
    int count = 0;
    int cellId = 0; 
    std::ostringstream oss;
    for (unsigned int index : indices)
    {
        count++;
        oss << index << " "; 
        if (count == 6)
        {
            qDebug() << "Vector elements for << " << cellId++ << ":" << QString::fromStdString(oss.str());
            oss.str("");
            oss.clear();
            count = 0;
        }
    }
*/
}
