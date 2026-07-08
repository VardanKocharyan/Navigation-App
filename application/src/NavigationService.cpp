#include "application/NavigationService.hpp"
#include "application/RoutePresentationBuilder.hpp"


namespace application
{


NavigationService::NavigationService(
    const core::graph::Graph& graph,
    std::unique_ptr<IPathFinder> pathFinder,
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


    result.success = true;
    result.message = "Route calculated successfully";
    result.route =
        builder.build(route);


    return result;
}


}
