#pragma once

#include <optional>

#include "core/types/Types.hpp"


namespace application
{

struct Coordinate
{
    double latitude{};
    double longitude{};
};



class INodeGeometryProvider
{

public:

    virtual ~INodeGeometryProvider() = default;


    virtual std::optional<Coordinate> getCoordinate(
        core::types::NodeId nodeId
    ) const = 0;

};

}
