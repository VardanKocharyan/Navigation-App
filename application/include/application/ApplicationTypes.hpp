#pragma once

#include <string>

#include "core/model/Route.hpp"


namespace application
{

struct RouteResult
{
    bool success{false};

    core::model::Route route;

    std::string errorMessage;
};

}
