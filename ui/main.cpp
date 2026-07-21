#include <QApplication>

#include <application/ApplicationBootstrap.hpp>
#include <application/NavigationService.hpp>

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


    MainWindow window(
        bootstrap.navigationService(),
        bootstrap.graph(),
        bootstrap.nodeCoordinates()
    );


    window.show();


    return app.exec();
}
