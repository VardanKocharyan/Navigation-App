#include "internal/GraphBuilder.hpp"

#include "core/model/Edge.hpp"
#include "core/model/Node.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>


namespace persistence::internal
{

namespace
{

constexpr double kCoordinatePrecision =
    1'000'000.0;


std::int64_t
quantize(
    double value
)
{
    return static_cast<std::int64_t>(
        std::llround(
            value * kCoordinatePrecision
        )
    );
}

}


// CoordinateHash

std::size_t
GraphBuilder::CoordinateHash::operator()(
    const Coordinate& coordinate
) const noexcept
{
    const auto lon =
        std::hash<std::int64_t>{}(
            quantize(
                coordinate.longitude
            )
        );


    const auto lat =
        std::hash<std::int64_t>{}(
            quantize(
                coordinate.latitude
            )
        );


    return lon ^ (lat << 1);
}


// CoordinateEqual

bool
GraphBuilder::CoordinateEqual::operator()(
    const Coordinate& lhs,
    const Coordinate& rhs
) const noexcept
{
    return
        quantize(lhs.longitude)
            ==
        quantize(rhs.longitude)

        &&

        quantize(lhs.latitude)
            ==
        quantize(rhs.latitude);
}


// resolveNode

core::types::NodeId
GraphBuilder::resolveNode(
    const Coordinate& coordinate,
    CoordinateIndex& index,
    std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >& nodeCoordinates,
    core::graph::Graph& graph,
    core::types::NodeId& nextNodeId
)
{
    const auto iterator =
        index.find(coordinate);


    if (iterator != index.end())
    {
        return iterator->second;
    }


    const auto nodeId =
        nextNodeId++;


    const bool inserted =
        graph.addNode(
            core::model::Node{
                nodeId
            }
        );


    if (!inserted)
    {
        throw std::runtime_error(
            "GraphBuilder: failed to add unique node"
        );
    }


    index.emplace(
        coordinate,
        nodeId
    );


    nodeCoordinates.emplace(
        nodeId,
        persistence::Coordinate{
            coordinate.longitude,
            coordinate.latitude
        }
    );


    return nodeId;
}


// distance

double
GraphBuilder::distance(
    const Coordinate& lhs,
    const Coordinate& rhs
)
{
    const auto dx =
        lhs.longitude -
        rhs.longitude;


    const auto dy =
        lhs.latitude -
        rhs.latitude;


    return std::sqrt(
        dx * dx +
        dy * dy
    );
}


// build

persistence::ImportedGraph
GraphBuilder::build(
    const std::vector<RoadFeature>& roads
) const
{
    persistence::ImportedGraph result;


    auto& graph =
        result.graph;


    auto& nodeCoordinates =
        result.nodeCoordinates;


    CoordinateIndex coordinateIndex;


    core::types::NodeId nextNodeId =
        1;


    core::types::EdgeId nextEdgeId =
        1;


    for (const auto& road : roads)
    {
        for (const auto& line : road.lines)
        {
            if (line.size() < 2)
            {
                continue;
            }


            for (
                std::size_t i = 1;
                i < line.size();
                ++i
            )
            {
                const auto& fromCoordinate =
                    line[i - 1];


                const auto& toCoordinate =
                    line[i];


                if (
                    fromCoordinate.longitude
                        ==
                    toCoordinate.longitude

                    &&

                    fromCoordinate.latitude
                        ==
                    toCoordinate.latitude
                )
                {
                    continue;
                }


                const auto from =
                    resolveNode(
                        fromCoordinate,
                        coordinateIndex,
                        nodeCoordinates,
                        graph,
                        nextNodeId
                    );


                const auto to =
                    resolveNode(
                        toCoordinate,
                        coordinateIndex,
                        nodeCoordinates,
                        graph,
                        nextNodeId
                    );


                const auto weight =
                    distance(
                        fromCoordinate,
                        toCoordinate
                    );


                graph.addEdge(
                    core::model::Edge{
                        nextEdgeId++,
                        from,
                        to,
                        weight
                    }
                );
            }
        }
    }


    return result;
}

}
