#pragma once

#include <unordered_map>

#include "core/graph/Graph.hpp"
#include "core/types/Types.hpp"

namespace persistence
{

struct Coordinate
{
    double longitude{};
    double latitude{};
};


struct ImportedGraph
{
    core::graph::Graph graph;

    std::unordered_map<
        core::types::NodeId,
        Coordinate
    > nodeCoordinates;
};

}
