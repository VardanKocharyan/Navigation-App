#include "application/NavigationService.hpp"
#include "application/RoutePresentationBuilder.hpp"
#include <iostream>


namespace application
{


NavigationService::NavigationService(
    const core::graph::Graph& graph,
    std::unique_ptr<algorithms::IPathFinder> pathFinder,
    const INodeGeometryProvider& geometryProvider
)
    :
    graph_(graph),
    pathFinder_(std::move(pathFinder)),
    geometryProvider_(geometryProvider)
{
}


NavigationResponse NavigationService::calculateRoute(
    core::types::NodeId start,
    core::types::NodeId destination
)
{
    NavigationResponse result;


    if(!graph_.findNode(start))
    {
        result.success = false;
        result.message = "Invalid start node";

        return result;
    }


    if(!graph_.findNode(destination))
    {
        result.success = false;
        result.message = "Invalid destination node";

        return result;
    }


    core::model::Route route =
        pathFinder_->findPath(
            graph_,
            start,
            destination
        );

    if(route.path().empty())
    {
        result.success = false;
        result.message = "Route unavailable";

        return result;
    }


    RoutePresentationBuilder builder(
        geometryProvider_
    );


   auto presentation =
        builder.build(route);

    result.success = true;
    result.message = "Route calculated successfully";
    result.route =
        std::move(presentation);

    return result;
}


MultiDestinationNavigationResponse
NavigationService::requestMultiDestinationRoute(
    core::types::NodeId start,
    const std::vector<core::types::NodeId>& destinations
)
{
    MultiDestinationNavigationResponse result;


    if (destinations.empty())
    {
        result.success = false;
        result.message =
            "No destinations provided";

        return result;
    }


    if (!graph_.findNode(start))
    {
        result.success = false;
        result.message =
            "Invalid start node";

        return result;
    }


    core::types::NodeId current =
        start;


    for (
        std::size_t index = 0;
        index < destinations.size();
        ++index
    )
    {
        const auto destination =
            destinations[index];

        std::cout
            << "Multi segment: "
            << index
            << " | source: "
            << current
            << " | destination: "
            << destination
            << std::endl;


        if (!graph_.findNode(destination))
        {
            result.success = false;

            result.message =
                "Invalid destination node";


            result.failure =
                NavigationFailure{
                    index,
                    current,
                    destination,
                    "Invalid destination node"
                };


            return result;
        }


        if (current == destination)
        {
            std::cout
                << "IDENTICAL DESTINATIONS DETECTED"
                << std::endl;
    
            result.success = false;

            result.message =
                "Consecutive identical destinations";

            result.failure =
                NavigationFailure{
                    index,
                    current,
                    destination,
                    "Consecutive identical destinations"
                };

            return result;
        }


        core::model::Route route =
            pathFinder_->findPath(
                graph_,
                current,
                destination
            );


        if (route.path().empty())
        {
            result.success = false;

            result.message =
                "Route unavailable";


            result.failure =
                NavigationFailure{
                    index,
                    current,
                    destination,
                    "Route unavailable"
                };


            return result;
        }


        result.segments.push_back(
            NavigationSegment{
                current,
                destination,
                std::move(route)
            }
        );


        current =
            destination;
    }


    result.success = true;

    result.message =
        "Multi-destination route calculated successfully";


    return result;
}

}
