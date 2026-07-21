#pragma once

#include <core/graph/Graph.hpp>
#include <core/model/Route.hpp>
#include <core/types/Types.hpp>


namespace algorithms
{

class IPathFinder
{
public:

    virtual ~IPathFinder() = default;


    [[nodiscard]]
    virtual core::model::Route findPath(
        const core::graph::Graph& graph,
        core::types::NodeId source,
        core::types::NodeId destination
    ) const = 0;
};

}
