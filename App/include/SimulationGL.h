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

class SimulationGL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SimulationGL(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SimulationGL();

public slots:
    void loadMesh(const fstim::Mesh* mesh);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    vecp::Vec2f m_length{1.f, 1.f}; 
    int m_nCells = -1;
    QTimer* m_timer;
    unsigned int m_buffer = 0;
    
    std::mutex renderMutex;
    bool m_isMeshLoaded = false;

    void m_updateCanvas();
    void m_drawRectangle();

};