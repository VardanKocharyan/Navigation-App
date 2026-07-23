#pragma once

#include <memory>
#include <unordered_map>

#include "core/types/Types.hpp"
#include "ImportedGraph.hpp"

namespace core::graph
{
class Graph;
}

namespace algorithms
{
class IPathFinder;
}

namespace application
{

class GraphLoader;
class NavigationService;
class RuntimeGeometryProvider;

class ApplicationBootstrap
{
public:

    ApplicationBootstrap();


    ~ApplicationBootstrap();


    NavigationService&
    navigationService();


    const core::graph::Graph&
    graph() const;


    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >&
    nodeCoordinates() const;


private:

    std::unique_ptr<GraphLoader>
        graphLoader;


    std::unique_ptr<algorithms::IPathFinder>
        pathFinder;


    std::unique_ptr<RuntimeGeometryProvider>
        geometryProvider;


    std::unique_ptr<NavigationService>
        service;

};

}
