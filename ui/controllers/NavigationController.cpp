#include "NavigationController.hpp"

#include <application/NavigationService.hpp>

#include <QDebug>

NavigationController::NavigationController(
    application::NavigationService& service,
    QObject* parent
)
    :
    QObject(parent),
    navigationService(service)
{
}


void NavigationController::requestRoute(
    core::types::NodeId start,
    core::types::NodeId destination
)
{

    qDebug() << "NavigationController::requestRoute called";

    auto response =
        navigationService.calculateRoute(
            start,
            destination
        );


    if(response.success)
    {
        if(response.route.has_value())
        {
            emit routeReady(
                *response.route
            );

            qDebug()
            << "UI received geometry points:"
            << response.route->geometryPoints.size();
        }
    }
    else
    {
        emit navigationFailed(
            QString::fromStdString(
                response.message
            )
        );
    }
}
