#include "MapRenderer.h"

#include <algorithm>
#include <limits>

#include <QDebug>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPen>
#include <QRectF>
#include <QBrush>

#include <application/RoutePresentationData.hpp>


MapRenderer::MapRenderer(
    QGraphicsScene* scene
)
    :
    scene(scene)
{
}


void
MapRenderer::renderGraph(
    const core::graph::Graph& graph,
    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >& nodeCoordinates
)
{
    if (!scene)
    {
        return;
    }


    if (roadNetworkItem)
    {
        scene->removeItem(
            roadNetworkItem
        );


        delete roadNetworkItem;


        roadNetworkItem =
            nullptr;
    }


    if (nodeCoordinates.empty())
    {
        return;
    }


    minLongitude_ =
        std::numeric_limits<double>::max();


    minLatitude_ =
        std::numeric_limits<double>::max();

    double maxLongitude =
        std::numeric_limits<double>::lowest();


    double maxLatitude =
        std::numeric_limits<double>::lowest();


    for (const auto& entry :
         nodeCoordinates)
    {
        const auto& coordinate =
            entry.second;


        minLongitude_ =
            std::min(
                minLongitude_,
                coordinate.longitude
            );


        minLatitude_ =
            std::min(
                minLatitude_,
                coordinate.latitude
            );

        maxLongitude =
            std::max(
                maxLongitude,
                coordinate.longitude
            );


        maxLatitude =
            std::max(
                maxLatitude,
                coordinate.latitude
            );
    }


    coordinateTransformReady_ =
        true;
 
    qDebug()
        << "minLongitude:"
        << minLongitude_
        << "maxLongitude:"
        << maxLongitude
        << "minLatitude:"
        << minLatitude_
        << "maxLatitude:"
        << maxLatitude;


    QPainterPath roadPath;


    for (const auto& edge :
         graph.edges())
    {
        const auto fromIterator =
            nodeCoordinates.find(
                edge.from()
            );


        const auto toIterator =
            nodeCoordinates.find(
                edge.to()
            );


        if (fromIterator ==
            nodeCoordinates.end())
        {
            continue;
        }


        if (toIterator ==
            nodeCoordinates.end())
        {
            continue;
        }


        const QPointF fromPoint =
            toScenePoint(
                fromIterator->second
            );


        const QPointF toPoint =
            toScenePoint(
                toIterator->second
            );


        roadPath.moveTo(
            fromPoint
        );


        roadPath.lineTo(
            toPoint
        );
    }


    if (roadPath.isEmpty())
    {
        return;
    }


    roadNetworkItem =
        scene->addPath(
            roadPath
        );


    if (!roadNetworkItem)
    {
        return;
    }


    QPen roadPen;


    roadPen.setWidthF(
        2.0
    );


    roadPen.setCosmetic(
        true
    );


    roadNetworkItem->setPen(
        roadPen
    );


    roadNetworkItem->setZValue(
        0.0
    );


    const QRectF networkBounds =
        roadNetworkItem->boundingRect();


    frameScene(
        networkBounds
    );


    scene->update();

}


void
MapRenderer::renderRoute(
    const application::RoutePresentationData& route
)
{
    clearRoute();


    if (!scene)
    {
        return;
    }


    if (!coordinateTransformReady_)
    {
        qDebug()
            << "coordinate transform is not ready";

        return;
    }


    if (route.geometryPoints.size() < 2)
    {
        qDebug()
            << "Route has fewer than two points";

        return;
    }


    QPainterPath path;


    bool firstPoint = true;

    for (const auto& point : route.geometryPoints)
    {
        const QPointF scenePoint =
            toScenePoint(point);

        if (firstPoint)
        {
            path.moveTo(scenePoint);
            firstPoint = false;
        }
        else
        {
            path.lineTo(scenePoint);
        }
    }

    routeItem =
        scene->addPath(
            path
        );

    if (!routeItem)
    {
        return;
    }


    QPen routePen;


    routePen.setWidthF(
        2.0
    );


    routePen.setCosmetic(
        true
    );


    routeItem->setPen(
        routePen
    );


    routeItem->setZValue(
        1.0
    );


    scene->update();

}

std::optional<
    persistence::Coordinate
>
MapRenderer::toGeographicPoint(
    const QPointF& scenePoint
) const
{
    if (!coordinateTransformReady_)
    {
        return std::nullopt;
    }


    persistence::Coordinate coordinate;


    coordinate.longitude =
        (
            scenePoint.x() /
            coordinateScale_
        )
        +
        minLongitude_;


    coordinate.latitude =
        minLatitude_
        -
        (
            scenePoint.y() /
            coordinateScale_
        );


    return coordinate;
}


QPointF
MapRenderer::toScenePoint(
    const persistence::Coordinate& coordinate
) const
{
    const double x =
        (
            coordinate.longitude -
            minLongitude_
        )
        *
        coordinateScale_;


    const double y =
        -(
            coordinate.latitude -
            minLatitude_
        )
        *
        coordinateScale_;


    return QPointF(
        x,
        y
    );
}

QPointF
MapRenderer::toScenePoint(
    const application::Coordinate& coordinate
) const
{
    const double x =
        (
            coordinate.longitude -
            minLongitude_
        )
        *
        coordinateScale_;


    const double y =
        -(
            coordinate.latitude -
            minLatitude_
        )
        *
        coordinateScale_;


    return QPointF(
        x,
        y
    );
}

void
MapRenderer::frameScene(
    const QRectF& bounds
)
{
    if (!scene)
    {
        return;
    }


    if (!bounds.isValid())
    {
        return;
    }


    constexpr qreal marginFactor =
        0.05;


    const qreal horizontalMargin =
        std::max(
            bounds.width() *
                marginFactor,
            10.0
        );


    const qreal verticalMargin =
        std::max(
            bounds.height() *
                marginFactor,
            10.0
        );


    const QRectF framedBounds =
        bounds.adjusted(
            -horizontalMargin,
            -verticalMargin,
            horizontalMargin,
            verticalMargin
        );


    scene->setSceneRect(
        framedBounds
    );


    const auto views =
        scene->views();


    if (!views.isEmpty())
    {
        QGraphicsView* view =
            views.front();

        view->fitInView(
            framedBounds,
            Qt::KeepAspectRatio
        );

    }
}


void
MapRenderer::clearRoute()
{
    if (!routeItem)
    {
        return;
    }


    if (scene)
    {
        scene->removeItem(
            routeItem
        );
    }


    delete routeItem;


    routeItem =
        nullptr;
}

void
MapRenderer::setStartMarker(
    const persistence::Coordinate& coordinate
)
{
    if (!scene)
    {
        return;
    }


    if (startMarkerItem)
    {
        scene->removeItem(
            startMarkerItem
        );


        delete startMarkerItem;


        startMarkerItem =
            nullptr;
    }


    const QPointF scenePoint =
        toScenePoint(
            coordinate
        );


    constexpr double markerSize =
        12.0;


    startMarkerItem =
        scene->addEllipse(
            scenePoint.x() -
                markerSize / 2.0,
            scenePoint.y() -
                markerSize / 2.0,
            markerSize,
            markerSize
        );


    if (!startMarkerItem)
    {
        return;
    }


    startMarkerItem->setBrush(
        QBrush(
            Qt::green
        )
    );


    startMarkerItem->setZValue(
        2.0
    );
}

void
MapRenderer::setDestinationMarker(
    const persistence::Coordinate& coordinate
)
{
    if (!scene)
    {
        return;
    }


    if (destinationMarkerItem)
    {
        scene->removeItem(
            destinationMarkerItem
        );


        delete destinationMarkerItem;


        destinationMarkerItem =
            nullptr;
    }


    const QPointF scenePoint =
        toScenePoint(
            coordinate
        );


    constexpr double markerSize =
        12.0;


    destinationMarkerItem =
        scene->addEllipse(
            scenePoint.x() -
                markerSize / 2.0,
            scenePoint.y() -
                markerSize / 2.0,
            markerSize,
            markerSize
        );


    if (!destinationMarkerItem)
    {
        return;
    }


    destinationMarkerItem->setBrush(
        QBrush(
            Qt::red
        )
    );


    destinationMarkerItem->setZValue(
        2.0
    );
}

void
MapRenderer::clearMarkers()
{
    if (!scene)
    {
        return;
    }


    if (startMarkerItem)
    {
        scene->removeItem(
            startMarkerItem
        );


        delete startMarkerItem;


        startMarkerItem =
            nullptr;
    }


    if (destinationMarkerItem)
    {
        scene->removeItem(
            destinationMarkerItem
        );


        delete destinationMarkerItem;


        destinationMarkerItem =
            nullptr;
    }
}

void
MapRenderer::fitGraphInView()
{

    if (!scene ||
        !roadNetworkItem)
    {
        qDebug()
            << "fitGraphInView aborted";

        return;
    }

    const auto views =
        scene->views();

    if (views.isEmpty())
    {
        qDebug()
            << "fitGraphInView: no views";

        return;
    }

    QGraphicsView* view =
        views.front();

    view->fitInView(
        roadNetworkItem->boundingRect(),
        Qt::KeepAspectRatio
    );

}
