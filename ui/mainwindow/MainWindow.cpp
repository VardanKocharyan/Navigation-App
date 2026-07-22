#include "MainWindow.h"

#include "../controllers/SceneController.h"
#include "../controllers/NavigationController.hpp"
#include "../rendering/MapRenderer.h"

#include <application/NavigationService.hpp>
#include <application/RoutePresentationData.hpp>

#include <core/graph/Graph.hpp>

#include <limits>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QShowEvent>
#include <QTimer>
#include <QPointF>

MainWindow::MainWindow(
    application::NavigationService& navigationService,
    const core::graph::Graph& graph,
    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >& nodeCoordinates,
    QWidget* parent
)
    :
    QMainWindow(parent),
    graph_(graph),
    nodeCoordinates_(nodeCoordinates),
    scene(nullptr),
    graphicsView(nullptr),
    sceneController(nullptr),
    navigationController(nullptr),
    mapRenderer(nullptr)
{

    qDebug()
        << "MainWindow coordinates:"
        << nodeCoordinates_.size();

    setWindowTitle(
        "Yerevan Navigation System"
    );

    scene =
        new QGraphicsScene(
            this
        );


    graphicsView =
        new QGraphicsView(
            this
        );


    graphicsView->setScene(
        scene
    );


    sceneController =
        new SceneController(
            scene
        );

    connect(
        sceneController,
        &SceneController::sceneClicked,
        this,
        &MainWindow::handleSceneClick
    );


    mapRenderer =
        new MapRenderer(
            scene
        );


    mapRenderer->renderGraph(
        graph_,
        nodeCoordinates_
    );


    navigationController =
        new NavigationController(
            navigationService,
            this
        );


    connect(
        navigationController,
        &NavigationController::routeReady,
        this,
        &MainWindow::onRouteReady
    );


    connect(
        navigationController,
        &NavigationController::navigationFailed,
        this,
        &MainWindow::onNavigationFailed
    );


    setCentralWidget(
        graphicsView
    );

    QTimer::singleShot(
        0,
        this,
        [this]()
        {
            mapRenderer->fitGraphInView();
        }
    );
}

void
MainWindow::handleSceneClick(
    double x,
    double y
)
{
    const QPointF scenePoint(
        x,
        y
    );


    qDebug()
        << "Scene clicked:"
        << scenePoint;


    const auto geographicPoint =
        mapRenderer->toGeographicPoint(
            scenePoint
        );


    if (!geographicPoint.has_value())
    {
        qDebug()
            << "Could not convert scene point"
            << "to geographic coordinate";

        return;
    }


    qDebug()
        << "Geographic point:"
        << geographicPoint->longitude
        << geographicPoint->latitude;


    const auto nearestNodeId =
        findNearestNode(
            *geographicPoint
        );


    if (!nearestNodeId.has_value())
    {
        qDebug()
            << "Could not find nearest graph node";

        return;
    }


    qDebug()
        << "Nearest NodeId:"
        << *nearestNodeId;


    if (!selectedStartNode_.has_value())
    {
        selectedStartNode_ =
            *nearestNodeId;

        mapRenderer->setStartMarker(
            *geographicPoint
        );


        qDebug()
            << "Start node selected:"
            << *selectedStartNode_;


        statusBar()->showMessage(
            "Start point selected. "
            "Select destination."
        );


        return;
    }


    const auto startNodeId =
        *selectedStartNode_;


    const auto destinationNodeId =
        *nearestNodeId;


    qDebug()
        << "Destination node selected:"
        << destinationNodeId;


    if (startNodeId ==
        destinationNodeId)
    {
        qDebug()
            << "Start and destination are identical";


        mapRenderer->clearMarkers();


        selectedStartNode_.reset();


        statusBar()->showMessage(
            "Start and destination cannot be identical."
        );


        return;
    }

    mapRenderer->setDestinationMarker(
        *geographicPoint
    );

    const auto startOutgoingEdges =
        graph_.getOutgoingEdges(
            startNodeId
        );


    const auto destinationOutgoingEdges =
        graph_.getOutgoingEdges(
            destinationNodeId
        );

    navigationController->requestRoute(
        startNodeId,
        destinationNodeId
    );

    selectedStartNode_.reset();

    statusBar()->showMessage(
        "Route requested."
    );
}


std::optional<
    core::types::NodeId
>
MainWindow::findNearestNode(
    const persistence::Coordinate& coordinate
) const
{
    if (nodeCoordinates_.empty())
    {
        return std::nullopt;
    }

    core::types::NodeId nearestNodeId{};

    double minimumDistance =
        std::numeric_limits<double>::max();

    bool nodeFound = false;

    for (const auto& [nodeId, nodeCoordinate] :
         nodeCoordinates_)
    {
        const double longitudeDifference =
            nodeCoordinate.longitude -
            coordinate.longitude;

        const double latitudeDifference =
            nodeCoordinate.latitude -
            coordinate.latitude;

        const double distanceSquared =
            longitudeDifference *
                longitudeDifference
            +
            latitudeDifference *
                latitudeDifference;

        if (distanceSquared <
            minimumDistance)
        {
            minimumDistance =
                distanceSquared;

            nearestNodeId =
                nodeId;

            nodeFound =
                true;
        }
    }

    constexpr double MAX_SNAP_DISTANCE_SQUARED =
        0.01 * 0.01;

    if (!nodeFound ||
        minimumDistance >
            MAX_SNAP_DISTANCE_SQUARED)
    {
        qDebug()
            << "No graph node within snap distance:"
            << minimumDistance;

        return std::nullopt;
    }

    qDebug()
        << "Nearest candidate:"
        << nearestNodeId
        << "distanceSquared:"
        << minimumDistance;

    return nearestNodeId;
}

void
MainWindow::onNavigationFailed(
    const QString& message
)
{
    statusBar()->showMessage(
        message
    );
}


void
MainWindow::onRouteReady(
    const application::RoutePresentationData& route
)
{
    mapRenderer->renderRoute(
        route
    );
}

void
MainWindow::showEvent(
    QShowEvent* event
)
{
    QMainWindow::showEvent(
        event
    );

    mapRenderer->fitGraphInView();
}




