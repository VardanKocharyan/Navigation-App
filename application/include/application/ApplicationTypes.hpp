#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "core/model/Route.hpp"
#include "core/types/Types.hpp"


namespace application
{

struct RouteResult
{
    bool success{false};

    core::model::Route route;

    std::string errorMessage;
};


using DestinationList =
    std::vector<core::types::NodeId>;


struct NavigationSegment
{
    core::types::NodeId source;

    core::types::NodeId destination;

    core::model::Route route;
};


struct NavigationFailure
{
    std::size_t segmentIndex;

    core::types::NodeId source;

    core::types::NodeId destination;

    std::string message;
};


struct MultiDestinationNavigationResponse
{
    bool success{false};

    std::string message;

    std::vector<NavigationSegment> segments;

    std::optional<NavigationFailure> failure;
};

}
