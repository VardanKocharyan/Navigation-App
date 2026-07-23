#pragma once

#include <vector>

#include "application/RoutePresentationData.hpp"

namespace application
{

class RoutePresentationAggregator
{
public:

    RoutePresentationData aggregate(
        const std::vector<RoutePresentationData>& segments
    ) const;
};

}
