#pragma once

#include <QObject>

#include <application/RoutePresentationData.hpp>

#include <core/types/Types.hpp>

#include <vector>

namespace application
{
class NavigationService;
}

class NavigationController : public QObject
{
    Q_OBJECT

public:

    explicit NavigationController(
        application::NavigationService& service,
        QObject* parent = nullptr
    );

public slots:

    void requestRoute(
        core::types::NodeId start,
        core::types::NodeId destination
    );

    void requestMultiDestinationRoute(
        core::types::NodeId start,
        const std::vector<
            core::types::NodeId
        >& destinations
    );

signals:

    void routeReady(
        const application::RoutePresentationData& route
    );

    void navigationFailed(
        const QString& message
    );

    void multiDestinationRouteReady(
        const application::RoutePresentationData& route
    );

private:

    application::NavigationService& navigationService;
};
