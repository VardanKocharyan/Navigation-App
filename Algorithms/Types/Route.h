#pragma once

#include <vector>

using NodeId = int;

struct Route
{
    std::vector<NodeId> nodes;
    double totalDistance = 0.0;
};
