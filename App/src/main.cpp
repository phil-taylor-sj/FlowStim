
#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>
#include <QtGUI>

#include <Core/Domain/Cell.h>
#include <SimulationGL.h>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //QFont font("Courier");
    //QWindow window_1;
    //QPushButton button_1 ("Hello world!");
    //window_1.resize(500, 500);
    //window_1.show();
    QWidget window;
    window.resize(800, 600);

    std::unique_ptr<QVBoxLayout> layout = std::make_unique<QVBoxLayout>(&window);
    std::unique_ptr<QPushButton> button = std::make_unique<QPushButton>("Hello world!");

    std::unique_ptr<SimulationGL> simulation = std::make_unique<SimulationGL>();
    simulation->setMinimumSize(800, 600);
    
    layout->addWidget(simulation.get());
    layout->addWidget(button.get());
    window.setLayout(layout.get());
    window.show();
    return app.exec();
} 
