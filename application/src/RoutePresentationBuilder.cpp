#include "application/RoutePresentationBuilder.hpp"


namespace application
{


RoutePresentationBuilder::RoutePresentationBuilder(
    const INodeGeometryProvider& geometryProvider
)
    :
    geometryProvider_(geometryProvider)
{
}


RoutePresentationData RoutePresentationBuilder::build(
    const core::model::Route& route
) const
{
    RoutePresentationData result;


    for (auto nodeId : route.path())
    {
        auto coordinate =
            geometryProvider_.getCoordinate(nodeId);


        if (coordinate)
        {
            result.geometryPoints.push_back(*coordinate);
        }
    }


    /*
     * Core::Route currently does not contain
     * distance information.
     *
     * totalDistance remains default initialized.
     */

    return result;
}


}
