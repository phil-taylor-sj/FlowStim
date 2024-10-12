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
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }