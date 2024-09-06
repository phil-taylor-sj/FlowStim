
#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>
#include <QtGUI>

#include <Core/Domain/Cell.h>
#include <SimulationGL.h>
#include <Simulation2D.h>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //QFont font("Courier");

    QWidget window;
    window.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&window);
    QPushButton* startButton = new QPushButton("Start Solver");

    SimulationGL* simulation = new SimulationGL();
    simulation->setMinimumSize(800, 600);

    Simulation2D* solver = new Simulation2D();
    QObject::connect(startButton, &QPushButton::clicked, solver, &Simulation2D::start);
    QObject::connect(solver, &Simulation2D::renderMesh, simulation, &SimulationGL::loadMesh, Qt::QueuedConnection);

    layout->addWidget(simulation);
    layout->addWidget(startButton);
    window.setLayout(layout);
    window.show();
    return app.exec();
} 
