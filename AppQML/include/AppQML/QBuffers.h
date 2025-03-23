#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


struct QBuffers
{
    QOpenGLVertexArrayObject vertexArray{};
    QOpenGLBuffer colourBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};
    QOpenGLShaderProgram shaders{};

    QBuffers();
    ~QBuffers();
}