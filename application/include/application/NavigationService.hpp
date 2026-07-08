#pragma once

#include <memory>

#include "application/NavigationResponse.hpp"
#include "application/INodeGeometryProvider.hpp"

#include <vector>
#include <unordered_map>

#include "core/graph/Graph.hpp"
#include "core/types/Types.hpp"

#include "Interfaces/IPathFinder.hpp"


namespace application
{

class NavigationService
{

public:

    NavigationService(
        const core::graph::Graph& graph,
        std::unique_ptr<IPathFinder> pathFinder,
        const INodeGeometryProvider& geometryProvider
    );


    NavigationResponse calculateRoute(
        core::types::NodeId start,
        core::types::NodeId destination
    );


private:

    const core::graph::Graph& graph_;

    std::unique_ptr<IPathFinder> pathFinder_;

    const INodeGeometryProvider& geometryProvider_;

};


}
