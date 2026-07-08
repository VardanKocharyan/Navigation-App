#pragma once

#include "application/RoutePresentationData.hpp"
#include "application/INodeGeometryProvider.hpp"

#include "core/model/Route.hpp"


namespace application
{

class RoutePresentationBuilder
{
public:

    explicit RoutePresentationBuilder(
        const INodeGeometryProvider& geometryProvider
    );


    RoutePresentationData build(
        const core::model::Route& route
    ) const;


private:

    const INodeGeometryProvider& geometryProvider_;

};

}
