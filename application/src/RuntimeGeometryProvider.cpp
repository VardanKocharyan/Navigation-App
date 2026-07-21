#include "application/RuntimeGeometryProvider.hpp"


namespace application
{

RuntimeGeometryProvider::RuntimeGeometryProvider(
    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >& nodeCoordinates
)
    :
    nodeCoordinates_(
        nodeCoordinates
    )
{
}


std::optional<Coordinate>
RuntimeGeometryProvider::getCoordinate(
    core::types::NodeId nodeId
) const
{
    const auto iterator =
        nodeCoordinates_.find(
            nodeId
        );


    if (
        iterator
        ==
        nodeCoordinates_.end()
    )
    {
        return std::nullopt;
    }


    return Coordinate{
        iterator->second.latitude,
        iterator->second.longitude
    };
}

}
