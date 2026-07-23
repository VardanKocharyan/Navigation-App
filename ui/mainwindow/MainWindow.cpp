#include "MainWindow.h"

#include "../controllers/SceneController.h"
#include "../controllers/NavigationController.hpp"
#include "../rendering/MapRenderer.h"

#include <application/NavigationService.hpp>
#include <application/RoutePresentationData.hpp>

#include <core/graph/Graph.hpp>

#include <limits>
#include <algorithm>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QShowEvent>
#include <QTimer>
#include <QPointF>
#include <QPushButton>


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
    calculateRouteButton(nullptr),
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

    calculateRouteButton =
        new QPushButton(
            "Calculate Route",
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
        calculateRouteButton,
        &QPushButton::clicked,
        this,
        &MainWindow::calculateMultiDestinationRoute
    );

    connect(
        navigationController,
        &NavigationController::multiDestinationRouteReady,
        this,
        &MainWindow::onMultiDestinationRouteReady
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
            "Select destinations."
        );


        return;
    }


    const auto destinationNodeId =
        *nearestNodeId;


    if (
        destinationNodeId ==
        *selectedStartNode_
    )
    {
        statusBar()->showMessage(
            "Destination cannot be "
            "the same as the start."
        );

        return;
    }


    const auto alreadySelected =
        std::find(
            selectedDestinationNodes_.begin(),
            selectedDestinationNodes_.end(),
            destinationNodeId
        );


    if (
        alreadySelected !=
        selectedDestinationNodes_.end()
    )
    {
        statusBar()->showMessage(
            "Destination already selected."
        );

        return;
    }


    selectedDestinationNodes_.push_back(
        destinationNodeId
    );


    mapRenderer->setDestinationMarker(
        *geographicPoint
    );


    statusBar()->showMessage(
        QString(
            "Destination %1 selected."
        )
        .arg(
            selectedDestinationNodes_.size()
        )
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

void
MainWindow::onMultiDestinationRouteReady(
    const application::RoutePresentationData& route
)
{
    mapRenderer->renderRoute(
        route
    );


    selectedStartNode_.reset();


    selectedDestinationNodes_.clear();


    statusBar()->showMessage(
        "Multi-destination route ready."
    );
}

void
MainWindow::calculateMultiDestinationRoute()
{
    if (!selectedStartNode_.has_value())
    {
        statusBar()->showMessage(
            "Select a start point first."
        );

        return;
    }


    if (selectedDestinationNodes_.empty())
    {
        statusBar()->showMessage(
            "Select at least one destination."
        );

        return;
    }


    navigationController->
        requestMultiDestinationRoute(
            *selectedStartNode_,
            selectedDestinationNodes_
        );


    statusBar()->showMessage(
        "Multi-destination route requested."
    );
}
