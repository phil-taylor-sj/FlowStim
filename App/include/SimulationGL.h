#pragma once

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QTimer>
#include <mutex>
#include <iostream>

#include <QtGUI>
#include <VecPlus/Vec2.h>

#include <Core/Domain/MeshFactory.h>
#include <Core/Domain/Mesh.h>
#include <MeshData.h>

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
    unsigned int m_buffer = 0;
    
    std::shared_ptr<std::vector<vecp::Vec2f>> m_velocity = nullptr;

    std::mutex renderMutex;

    vecp::Vec2f m_domainLength = vecp::Vec2f(1., 1.);
    unsigned int m_nCells = 0;

    void m_updateCanvas();
    void m_drawMesh();
    void m_drawField();
};