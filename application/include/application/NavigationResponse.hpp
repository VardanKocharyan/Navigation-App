#pragma once

#include <optional>
#include <string>

#include "application/RoutePresentationData.hpp"

namespace application
{

struct NavigationResponse
{
    bool success{false};

    std::string message;

    std::optional<RoutePresentationData> route;
};

} // namespace application
