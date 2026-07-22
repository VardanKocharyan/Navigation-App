#pragma once

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QRectF>

#include <unordered_map>

#include <core/graph/Graph.hpp>
#include <core/types/Types.hpp>

#include <optional>

#include "ImportedGraph.hpp"


namespace application
{

class RoutePresentationData;

struct Coordinate;

}


class MapRenderer
{
public:

    explicit MapRenderer(
        QGraphicsScene* scene
    );


    void renderGraph(
        const core::graph::Graph& graph,
        const std::unordered_map<
            core::types::NodeId,
            persistence::Coordinate
        >& nodeCoordinates
    );


    void renderRoute(
        const application::RoutePresentationData& route
    );

    void clearRoute();

    void setStartMarker(
        const persistence::Coordinate& coordinate
    );


    void setDestinationMarker(
        const persistence::Coordinate& coordinate
    );

    void clearMarkers();

    std::optional<
        persistence::Coordinate
    >
    toGeographicPoint(
        const QPointF& scenePoint
    ) const;

    void fitGraphInView();


private:

    QPointF toScenePoint(
        const persistence::Coordinate& coordinate
    ) const;


    QPointF toScenePoint(
        const application::Coordinate& coordinate
    ) const;


    void frameScene(
        const QRectF& bounds
    );


private:

    QGraphicsScene* scene;


    QGraphicsPathItem*
        roadNetworkItem{nullptr};


    QGraphicsPathItem*
        routeItem{nullptr};

    QGraphicsEllipseItem*
        startMarkerItem{nullptr};


    QGraphicsEllipseItem*
        destinationMarkerItem{nullptr};


    double minLongitude_{};


    double minLatitude_{};


    bool coordinateTransformReady_{false};


    static constexpr double coordinateScale_ =
        100000.0;

};
