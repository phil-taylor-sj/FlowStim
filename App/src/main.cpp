
#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>
#include <QtGUI>
#include <QProgressBar>
#include <QOpenGLFunctions>
#include <QFile>

#include <Core/Domain/Cell.h>
#include <SimulationGL.h>
#include <Simulation2D.h>

int main(int argc, char **argv)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(4, 1); // macOS supports up to OpenGL 4.1
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app (argc, argv);
    
    //QFont font("Courier");
    QFile file(":/stylesheets/mainStyle.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString stylesheet = QLatin1String(file.readAll());
        app.setStyleSheet(stylesheet);
    }
    else
    {
        qWarning("Could not open stylesheet resource.");
    }


    QWidget window;
    window.resize(800, 600);
  
    SimulationGL* simulation = new SimulationGL();
    simulation->setMinimumSize(800, 600);

    Simulation2D* solver = new Simulation2D();
    
    QObject::connect(solver, &Simulation2D::sendMesh, 
        simulation, &SimulationGL::recieveMesh, Qt::QueuedConnection);
    
    QObject::connect(solver, &Simulation2D::sendVelocity, 
        simulation, &SimulationGL::recieveVelocity, Qt::QueuedConnection);

    QProgressBar* courantBar = new QProgressBar();
    courantBar->setOrientation(Qt::Vertical);

    QVBoxLayout* mainLayout = new QVBoxLayout(&window);
    
    QHBoxLayout* fieldLayout = new QHBoxLayout(&window);
    fieldLayout->addWidget(courantBar);
    fieldLayout->addWidget(simulation);
    
    mainLayout->addLayout(fieldLayout);
    
    QGridLayout* buttonGrid = new QGridLayout();
    
    QPushButton* startButton = new QPushButton("Start");
    QObject::connect(startButton, &QPushButton::clicked, 
        solver, &Simulation2D::start, Qt::QueuedConnection);

    QPushButton* stopButton = new QPushButton("Stop");
    QObject::connect(stopButton, &QPushButton::clicked, 
        solver, &Simulation2D::stop, Qt::QueuedConnection);

    QPushButton* resetButton = new QPushButton("Reset");
    QObject::connect(resetButton, &QPushButton::clicked, 
        solver, &Simulation2D::generate, Qt::QueuedConnection);
    
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 1, 0);
    buttonGrid->addWidget(resetButton, 2, 0);

    mainLayout->addLayout(buttonGrid);

    window.setLayout(mainLayout);
    window.show();
    return app.exec();
} 
