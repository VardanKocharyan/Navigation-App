#pragma once

#include <memory>
#include "IPathFinder.h"
#include "Types/AlgorithmType.h"

class PathFinderFactory
{
public:
    static std::unique_ptr<IPathFinder> create(AlgorithmType type);
};
