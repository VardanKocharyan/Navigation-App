#pragma once

#include <QMainWindow>

#include <QString>

#include <unordered_map>

#include <core/types/Types.hpp>

#include <optional>

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
class QShowEvent;

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

protected:

    void showEvent(
        QShowEvent* event
    ) override;

private slots:

    void onRouteReady(
        const application::RoutePresentationData& route
    );


    void onNavigationFailed(
        const QString& message
    );

    

private:

    std::optional<
        core::types::NodeId
    >
    findNearestNode(
        const persistence::Coordinate& coordinate
    ) const;

    std::optional<
        core::types::NodeId
    >
    selectedStartNode_;

    void handleSceneClick(
        double x,
        double y
    );


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
