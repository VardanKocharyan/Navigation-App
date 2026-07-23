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

void NavigationController::requestMultiDestinationRoute(
    core::types::NodeId start,
    const std::vector<
        core::types::NodeId
    >& destinations
)
{
    const auto response =
        navigationService.requestMultiDestinationRoute(
            start,
            destinations
        );

    if (response.success)
    {
        if (response.route.has_value())
        {
            emit routeReady(
                *response.route
            );

            return;
        }
    }

    emit navigationFailed(
        QString::fromStdString(
            response.message
        )
    );
}


