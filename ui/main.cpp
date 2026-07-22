#include <QApplication>

#include <application/ApplicationBootstrap.hpp>
#include <application/NavigationService.hpp>

#include <iostream>

#include "mainwindow/MainWindow.h"


int main(
    int argc,
    char* argv[]
)
{
    QApplication app(
        argc,
        argv
    );


    application::ApplicationBootstrap bootstrap;

    std::cout
        << "main coordinates: "
        << bootstrap.nodeCoordinates().size()
        << std::endl;

    MainWindow window(
        bootstrap.navigationService(),
        bootstrap.graph(),
        bootstrap.nodeCoordinates()
    );
 
    window.resize(
        1200,
        800
    );

    window.show();


    return app.exec();
}
