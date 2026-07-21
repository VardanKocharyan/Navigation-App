#include "application/ApplicationBootstrap.hpp"

#include "application/GraphLoader.hpp"
#include "application/NavigationService.hpp"
#include "application/RuntimeGeometryProvider.hpp"

#include <GeoJSONImporter.hpp>

#include <Factory/PathFinderFactory.hpp>
#include <Types/AlgorithmType.hpp>

#include <stdexcept>


namespace application
{

ApplicationBootstrap::ApplicationBootstrap()
{
    auto importer =
        std::make_unique<GeoJSONImporter>();


    graphLoader =
        std::make_unique<GraphLoader>(
            std::move(importer)
        );


    graphLoader->load(
        "persistence/tests/resources/export.geojson"
    );


    pathFinder =
        algorithms::PathFinderFactory::create(
            algorithms::AlgorithmType::Dijkstra
        );


    geometryProvider =
        std::make_unique<RuntimeGeometryProvider>(
            graphLoader->nodeCoordinates()
        );


    service =
        std::make_unique<NavigationService>(
            graphLoader->graph(),
            std::move(pathFinder),
            *geometryProvider
        );
}


ApplicationBootstrap::~ApplicationBootstrap() = default;


NavigationService&
ApplicationBootstrap::navigationService()
{
    return *service;
}


const core::graph::Graph&
ApplicationBootstrap::graph() const
{
    return graphLoader->graph();
}


const std::unordered_map<
    core::types::NodeId,
    persistence::Coordinate
>&
ApplicationBootstrap::nodeCoordinates() const
{
    return graphLoader->nodeCoordinates();
}

}
