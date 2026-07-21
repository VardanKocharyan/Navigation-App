#pragma once

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QPointF>
#include <QRectF>

#include <unordered_map>

#include <core/graph/Graph.hpp>
#include <core/types/Types.hpp>

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


    double minLongitude_{};


    double minLatitude_{};


    bool coordinateTransformReady_{false};


    static constexpr double coordinateScale_ =
        100000.0;

};
