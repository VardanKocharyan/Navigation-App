#pragma once

#include <vector>

#include "application/INodeGeometryProvider.hpp"


namespace application
{

struct RoutePresentationData
{
    std::vector<Coordinate> geometryPoints;

    double totalDistance{0.0};
};

}
