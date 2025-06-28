#pragma once

#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Domain/Mesh.h>
#include <MeshData.h>
#include <MeshGL.h>

#include <VecPlus/Vec2.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QDebug>
#include <QTimer>
#include <QtGUI>
//#include <QOpenGLFunctions_4_1_Core>

#include <iostream>
#include <vector>
#include <mutex>


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

    QOpenGLVertexArrayObject m_cellVao = QOpenGLVertexArrayObject();
    QOpenGLBuffer m_cellColourBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_cellVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_cellIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    QOpenGLVertexArrayObject m_gridVao = QOpenGLVertexArrayObject();
    QOpenGLBuffer m_gridColourBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_gridVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_gridIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    std::shared_ptr<std::vector<vecp::Vec2f>> m_velocity = nullptr;

    std::mutex m_mutex;

    vecp::Vec2f m_domainLength = vecp::Vec2f(1., 1.);
    unsigned int m_nCells = 0;
    unsigned int m_nVertices = 0;

    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    std::unique_ptr<QOpenGLShader> m_vertexShader;
    std::unique_ptr<QOpenGLShader> m_fragmentShader;

    int m_maxCell = 0;

    //QOpenGLFunctions* m_glFunctions = nullptr;

    void m_updateCanvas();
    void m_drawMesh();
    void m_drawField();
    void m_createShader();
    void m_createCellMesh(std::shared_ptr<MeshData>& data);
    void m_createGridMesh(std::shared_ptr<MeshData>& data);

    void m_deleteBuffers();
};