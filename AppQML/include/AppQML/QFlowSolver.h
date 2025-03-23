#pragma once

#include <QDebug>
#include <QTimer>

#include <chrono>
#include <thread>

#include <VecPlus/Vec2.h>
#include <Core/Domain/MeshFactory.h>
#include <Core/Domain/Mesh.h>
#include <Core/Solver/BurgersSolver.h>
#include <Core/Domain/FaceSetFactory.h>

#include <AppQML/MeshData.h>


class QFlowSolver : public QObject
{
    Q_OBJECT
public:
    void run();

    explicit QFlowSolver(QObject *parent = nullptr);
    ~QFlowSolver();

public slots:
    void start();
    void stop();
    void generate();

signals:
    void sendMesh(std::shared_ptr<MeshData> data);
    void sendField(std::shared_ptr<std::vector<vecp::Vec2f>> data);
    void sendMaxCFL();

private:
    vecp::Vec2d m_length = vecp::Vec2d(10., 20.);
    vecp::Vec2i m_size = vecp::Vec2i(20, 40);
    std::unique_ptr<fstim::BurgersSolver> m_solver = nullptr;
    
    QTimer* m_timer;
    std::atomic<bool> m_pause = true;

    std::mutex m_mutex;
    std::mutex m_timerMutex;

    void m_compute();
    void m_updateMeshData();
    void m_updateVelocityData();
};