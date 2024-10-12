#pragma once

#include <QOpenGLFunctions>

#include <MeshData.h>

class MeshGL
{
public:
    void static createVertexArray(unsigned int& vao, unsigned int& vbo, MeshData& data);
};