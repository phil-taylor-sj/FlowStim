#pragma once

#include <QOpenGLFunctions>

#include <sstream>

#include <MeshData.h>

class MeshGL
{
public:
    void static createVertexArray(unsigned int& vao, unsigned int& vbo, MeshData& data);

    void static createElementArray(unsigned int vao, unsigned int& ibo, MeshData& data);
};