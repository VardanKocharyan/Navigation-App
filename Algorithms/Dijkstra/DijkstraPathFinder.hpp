#pragma once

#include "../Interfaces/IPathFinder.hpp"


namespace algorithms
{

class DijkstraPathFinder final : public IPathFinder
{
public:

    core::model::Route findPath(
        const core::graph::Graph& graph,
        core::types::NodeId source,
        core::types::NodeId destination
    ) const override;
};

}
