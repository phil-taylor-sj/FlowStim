#pragma once

#include <QOpenGLWindow>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QTimer>

#include <VecPlus/Vec2.h>
#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Domain/Mesh.h>

class Simulation : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Simulation(QOpenGLWindow::UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
    ~Simulation();

signals:

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    std::unique_ptr<const fstim::Mesh2d> m_mesh;
    vecp::Vec2d m_length;
    
    QTimer* m_timer;
    
    void m_updateCanvas();
    void m_drawRectangle(const fstim::Cell2d& cell);

};