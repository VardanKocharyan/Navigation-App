#pragma once

#include <memory>

#include "application/ApplicationTypes.hpp"

#include <core/graph/Graph.hpp>

#include "Interfaces/IPathFinder.hpp"


namespace application
{

class RouteOptimizer
{
public:

    RouteOptimizer(
        const core::graph::Graph& graph,
        std::unique_ptr<algorithms::IPathFinder> pathFinder
    );


    OptimizationResult optimize(
        core::types::NodeId start,
        const DestinationList& destinations,
        OptimizationStrategy strategy
    ) const;


private:

    const core::graph::Graph& graph_;

    std::unique_ptr<algorithms::IPathFinder>
        pathFinder_;
};

}
