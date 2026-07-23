#include "application/RouteOptimizer.hpp"

#include <algorithm>
#include <limits>
#include <cmath>
#include <unordered_set>


namespace application
{


namespace
{

double calculateRouteCost(
    const core::graph::Graph& graph,
    const core::model::Route& route
)
{
    const auto& path = route.path();

    if (path.size() < 2)
    {
        return 0.0;
    }

    double totalCost = 0.0;

    for (std::size_t index = 0;
         index + 1 < path.size();
         ++index)
    {
        const auto source = path[index];
        const auto destination = path[index + 1];

        const auto outgoingEdges =
            graph.getOutgoingEdges(source);

        bool edgeFound = false;

        for (const auto& edge : outgoingEdges)
        {
            if (edge.to() == destination)
            {
                totalCost += edge.weight();
                edgeFound = true;
                break;
            }
        }

        if (!edgeFound)
        {
            return std::numeric_limits<double>::infinity();
        }
    }

    return totalCost;
}

}


RouteOptimizer::RouteOptimizer(
    const core::graph::Graph& graph,
    std::unique_ptr<algorithms::IPathFinder> pathFinder
)
    :
    graph_(graph),
    pathFinder_(std::move(pathFinder))
{
}


OptimizationResult RouteOptimizer::optimize(
    core::types::NodeId start,
    const DestinationList& destinations,
    OptimizationStrategy strategy
) const
{
    OptimizationResult result;

    if (destinations.empty())
    {
        result.failure =
            OptimizationFailure{
                "No destinations provided",
                std::nullopt
            };

        return result;
    }

    if (!graph_.findNode(start))
    {
        result.failure =
            OptimizationFailure{
                "Invalid start node",
                start
            };

        return result;
    }

    std::unordered_set<core::types::NodeId>
        seenDestinations;


    for (const auto destination : destinations)
    {
        if (!seenDestinations.insert(destination).second)
        {
            result.failure =
                OptimizationFailure{
                    "Duplicate destination",
                    destination
                };

            return result;
        }
    }


    for (const auto destination : destinations)
    {
        if (!graph_.findNode(destination))
        {
            result.failure =
                OptimizationFailure{
                    "Invalid destination node",
                    destination
                };

            return result;
        }
    }


    if (destinations.size() == 1)
    {
        result.success = true;
        result.orderedDestinations = destinations;

        return result;
    }


    if (strategy == OptimizationStrategy::Exact)
    {
        DestinationList candidate =
            destinations;

        DestinationList bestOrder;

        double bestCost =
            std::numeric_limits<double>::infinity();


        std::sort(
            candidate.begin(),
            candidate.end()
        );


        do
        {
            auto current = start;

            double totalCost = 0.0;

            bool valid = true;


            for (const auto destination : candidate)
            {
                const auto route =
                    pathFinder_->findPath(
                        graph_,
                        current,
                        destination
                    );


                if (route.path().empty())
                {
                    valid = false;
                    break;
                }


                const auto routeCost =
                    calculateRouteCost(
                        graph_,
                        route
                    );


                if (!std::isfinite(routeCost))
                {
                    valid = false;
                    break;
                }


                totalCost += routeCost;

                current = destination;
            }


            if (
                valid
                &&
                totalCost < bestCost
            )
            {
                bestCost = totalCost;
                bestOrder = candidate;
            }

        }
        while (
            std::next_permutation(
                candidate.begin(),
                candidate.end()
            )
        );


        if (bestOrder.empty())
        {
            result.failure =
                OptimizationFailure{
                    "No valid route found",
                    std::nullopt
                };

            return result;
        }


        result.success = true;
        result.orderedDestinations = bestOrder;

        return result;
    }


    result.failure =
        OptimizationFailure{
            "Unsupported optimization strategy",
            std::nullopt
        };

    return result;
}


}
