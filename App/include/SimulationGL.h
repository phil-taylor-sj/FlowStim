#pragma once

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
//#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <mutex>
#include <iostream>

#include <QtGUI>
#include <VecPlus/Vec2.h>

#include <Core/Domain/MeshFactory.h>
#include <Core/Domain/Mesh.h>
#include <MeshData.h>
#include <MeshGL.h>

#include <QSurfaceFormat>
#include <QOpenGLShader>

class SimulationGL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SimulationGL(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SimulationGL();

signals:

public slots:
    void recieveMesh(std::shared_ptr<MeshData> data);
    void recieveVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    QTimer* m_timer;

    QOpenGLVertexArrayObject m_vao = QOpenGLVertexArrayObject();
    QOpenGLBuffer m_colourBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    std::shared_ptr<std::vector<vecp::Vec2f>> m_velocity = nullptr;

    std::mutex m_mutex;

    vecp::Vec2f m_domainLength = vecp::Vec2f(1., 1.);
    unsigned int m_nCells = 0;

    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    std::unique_ptr<QOpenGLShader> m_vertexShader;
    std::unique_ptr<QOpenGLShader> m_fragmentShader;

    int m_maxCell = 0;

    void m_updateCanvas();
    void m_drawMesh();
    void m_drawField();
    void m_createShader();

    void m_deleteBuffers();
};