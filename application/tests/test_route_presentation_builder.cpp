#include <gtest/gtest.h>

#include "application/RoutePresentationBuilder.hpp"
#include "application/INodeGeometryProvider.hpp"

#include "core/model/Route.hpp"


namespace
{

class MockNodeGeometryProvider : public application::INodeGeometryProvider
{
public:

    std::optional<application::Coordinate> getCoordinate(
        core::types::NodeId nodeId
    ) const override
    {
        switch (nodeId)
        {
            case 1:
                return application::Coordinate{10.0, 20.0};

            case 2:
                return application::Coordinate{30.0, 40.0};

            case 3:
                return application::Coordinate{50.0, 60.0};

            default:
                return std::nullopt;
        }
    }
};

}


TEST(RoutePresentationBuilderTest, BuildsGeometryFromRoute)
{
    MockNodeGeometryProvider provider;

    application::RoutePresentationBuilder builder(provider);


    core::model::Route route(
        {
            1,
            2,
            3
        }
    );


    auto result = builder.build(route);


    ASSERT_EQ(result.geometryPoints.size(), 3);


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[0].latitude,
        10.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[0].longitude,
        20.0
    );


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[1].latitude,
        30.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[1].longitude,
        40.0
    );


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[2].latitude,
        50.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[2].longitude,
        60.0
    );
}
