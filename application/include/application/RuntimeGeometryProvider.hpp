#pragma once

#include "application/INodeGeometryProvider.hpp"

#include "ImportedGraph.hpp"

#include <unordered_map>


namespace application
{

class RuntimeGeometryProvider final
    : public INodeGeometryProvider
{
public:

    explicit RuntimeGeometryProvider(
        const std::unordered_map<
            core::types::NodeId,
            persistence::Coordinate
        >& nodeCoordinates
    );


    std::optional<Coordinate>
    getCoordinate(
        core::types::NodeId nodeId
    ) const override;


private:

    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >&
        nodeCoordinates_;
};

}
