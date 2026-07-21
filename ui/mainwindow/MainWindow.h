#pragma once

#include <QMainWindow>

#include <QString>

#include <unordered_map>

#include <core/types/Types.hpp>

namespace core::graph
{
class Graph;
}

namespace application
{
class NavigationService;
class RoutePresentationData;
}

namespace persistence
{
struct Coordinate;
}

class QGraphicsScene;
class QGraphicsView;

class SceneController;
class MapRenderer;
class NavigationController;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:

    explicit MainWindow(
        application::NavigationService& navigationService,
        const core::graph::Graph& graph,
        const std::unordered_map<
            core::types::NodeId,
            persistence::Coordinate
        >& nodeCoordinates,
        QWidget* parent = nullptr
    );


    ~MainWindow() override = default;


private slots:

    void onRouteReady(
        const application::RoutePresentationData& route
    );


    void onNavigationFailed(
        const QString& message
    );


private:

    const core::graph::Graph& graph_;


    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >& nodeCoordinates_;


    QGraphicsScene* scene;


    QGraphicsView* graphicsView;


    SceneController* sceneController;


    NavigationController* navigationController;


    MapRenderer* mapRenderer;

};
