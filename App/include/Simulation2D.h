#pragma once

#include <QWidget>
#include <QDebug>
#include <QTimer>

#include <VecPlus/Vec2.h>
#include <Core/Domain/MeshFactory.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/LaplaceSolver.h>
#include <Core/Domain/FaceSetFactory.h>

#include <MeshData.h>


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
    vecp::Vec2d m_length = vecp::Vec2d(100., 200.);
    vecp::Vec2i m_size = vecp::Vec2i(100, 200);
    std::unique_ptr<fstim::LaplaceSolver> m_solver = nullptr;
    
    QTimer* m_timer;

    std::mutex m_mutex;
    std::mutex m_timerMutex;

    void m_compute();
    void m_updateMeshData();
    void m_updateVelocityData();
};