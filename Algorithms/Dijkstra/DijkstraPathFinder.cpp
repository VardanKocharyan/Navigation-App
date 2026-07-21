#include "DijkstraPathFinder.hpp"

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <limits>

#include <core/graph/Graph.hpp>
#include <core/model/Route.hpp>


namespace algorithms
{

namespace
{

using NodeId = core::types::NodeId;
using Cost  = core::types::Weight;


struct NodeState
{
    NodeId node;
    Cost cost;


    NodeState(
        NodeId id,
        Cost distance
    )
        :
        node(id),
        cost(distance)
    {
    }


    bool operator>(const NodeState& other) const
    {
        if (cost == other.cost)
        {
            return node > other.node;
        }

        return cost > other.cost;
    }
};

}


core::model::Route
DijkstraPathFinder::findPath(
    const core::graph::Graph& graph,
    NodeId source,
    NodeId destination
) const
{
    constexpr Cost INF =
        std::numeric_limits<Cost>::infinity();


    std::priority_queue<
        NodeState,
        std::vector<NodeState>,
        std::greater<NodeState>
    > queue;


    std::unordered_map<NodeId, Cost> distance;

    std::unordered_map<NodeId, NodeId> parent;

    std::unordered_set<NodeId> visited;


    distance[source] = 0;

    queue.emplace(source, 0);


    while (!queue.empty())
    {
        const NodeState current = queue.top();

        queue.pop();


        if (visited.contains(current.node))
            continue;


        visited.insert(current.node);


        if (current.node == destination)
            break;


        const auto outgoingEdges =
            graph.getOutgoingEdges(current.node);


        for (const auto& edge : outgoingEdges)
        {
            NodeId next = edge.to();

            Cost weight = edge.weight();


            Cost newDistance =
                distance[current.node] + weight;


            auto existing =
                distance.find(next);


            if (existing == distance.end() ||
                newDistance < existing->second)
            {
                distance[next] = newDistance;

                parent[next] = current.node;

                queue.emplace(next, newDistance);
            }
        }
    }


    if (!distance.contains(destination))
    {
        return core::model::Route{};
    }


    core::model::Route::Path path;


    NodeId current = destination;


    while (true)
    {
        path.push_back(current);


        if (current == source)
            break;


        auto parentIt =
            parent.find(current);


        if (parentIt == parent.end())
        {
            return core::model::Route{};
        }


        current = parentIt->second;
    }


    std::reverse(
        path.begin(),
        path.end()
    );


    return core::model::Route(
        std::move(path)
    );
}

}
