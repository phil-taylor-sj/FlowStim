#pragma once

#include <QWidget>
#include <QDebug>
#include <QTimer>

#include <VecPlus/Vec2.h>
#include <Core/Domain/MeshFactory.h>
#include <Core/Domain/Mesh.h>

class Simulation2D : public QObject
{
    Q_OBJECT
public:
    explicit Simulation2D(QObject *parent = nullptr);
    ~Simulation2D();

signals:
    void renderMesh(const fstim::Mesh* mesh);

public slots:
    void start();

private:
    vecp::Vec2d m_length;
    std::unique_ptr<const fstim::Mesh> m_mesh;
    QTimer* m_timer;
    void m_sendMesh();

};