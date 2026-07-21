#include "MainWindow.h"

#include "../controllers/SceneController.h"
#include "../controllers/NavigationController.hpp"
#include "../rendering/MapRenderer.h"

#include <application/NavigationService.hpp>
#include <application/RoutePresentationData.hpp>

#include <core/graph/Graph.hpp>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QTimer>

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
        [this](
            const application::RoutePresentationData& route
        )
        {
            mapRenderer->renderRoute(
                route
            );
        }
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
