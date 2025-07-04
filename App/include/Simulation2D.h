#pragma once

#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/BurgersSolver.h>
#include <Core/Domain/FaceSetFactory.h>
#include <MeshData.h>

#include <VecPlus/Vec2.h>

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <mutex>

class Simulation2D : public QObject
{
    Q_OBJECT
public:
    explicit Simulation2D(QObject *parent = nullptr);
    ~Simulation2D();

public slots:
    void start();
    void stop();
    void generate();

signals:
    void sendMesh(std::shared_ptr<MeshData> data);
    void sendVelocity(std::shared_ptr<std::vector<vecp::Vec2f>> data);
    void sendMaxCFL();

private:
    vecp::Vec2d m_length = vecp::Vec2d(10., 20.);
    vecp::Vec2i m_size = vecp::Vec2i(20, 40);
    std::unique_ptr<fstim::SolverBase> m_solver = nullptr;
    
    QTimer* m_timer;

    std::mutex m_mutex;
    std::mutex m_timerMutex;

    void m_compute();
    void m_updateMeshData();
    void m_updateVelocityData();
};