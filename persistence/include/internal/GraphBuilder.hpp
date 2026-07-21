#pragma once

#include "RoadFeatureExtractor.hpp"

#include "ImportedGraph.hpp"

#include <cstdint>
#include <unordered_map>


namespace persistence::internal
{

class GraphBuilder
{
public:

    persistence::ImportedGraph
    build(
        const std::vector<RoadFeature>& roads
    ) const;


private:

    struct CoordinateHash
    {
        std::size_t operator()(
            const Coordinate& coordinate
        ) const noexcept;
    };


    struct CoordinateEqual
    {
        bool operator()(
            const Coordinate& lhs,
            const Coordinate& rhs
        ) const noexcept;
    };


    using CoordinateIndex =
        std::unordered_map<
            Coordinate,
            core::types::NodeId,
            CoordinateHash,
            CoordinateEqual
        >;


    static core::types::NodeId
    resolveNode(
        const Coordinate& coordinate,
        CoordinateIndex& index,
        std::unordered_map<
            core::types::NodeId,
            persistence::Coordinate
        >& nodeCoordinates,
        core::graph::Graph& graph,
        core::types::NodeId& nextNodeId
    );


    static double
    distance(
        const Coordinate& lhs,
        const Coordinate& rhs
    );
};

}
