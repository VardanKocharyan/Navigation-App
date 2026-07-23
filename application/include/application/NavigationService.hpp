#pragma once

#include <memory>

#include "application/NavigationResponse.hpp"
#include "application/INodeGeometryProvider.hpp"
#include "application/RoutePresentationBuilder.hpp"
#include "application/RoutePresentationAggregator.hpp"

#include <core/graph/Graph.hpp>
#include <core/types/Types.hpp>

#include "Interfaces/IPathFinder.hpp"
#include "application/ApplicationTypes.hpp"


namespace application
{

class NavigationService
{
public:

    NavigationService(
        const core::graph::Graph& graph,
        std::unique_ptr<algorithms::IPathFinder> pathFinder,
        const INodeGeometryProvider& geometryProvider
    );

    NavigationResponse calculateRoute(
        core::types::NodeId start,
        core::types::NodeId destination
    );

    MultiDestinationNavigationResponse
    requestMultiDestinationRoute(
        core::types::NodeId start,
        const std::vector<core::types::NodeId>& destinations
    );

private:

    const core::graph::Graph& graph_;

    std::unique_ptr<algorithms::IPathFinder> pathFinder_;

    const INodeGeometryProvider& geometryProvider_;
};

}
