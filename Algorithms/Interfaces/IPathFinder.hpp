#pragma once

#include "Types/Route.h"

class Graph; // forward declaration

class IPathFinder
{
public:
    virtual ~IPathFinder() = default;

    virtual Route findPath(
        const Graph& graph,
        NodeId source,
        NodeId destination
    ) = 0;
};
