#include <gtest/gtest.h>

#include "application/RoutePresentationAggregator.hpp"


namespace
{

application::Coordinate point(
    double latitude,
    double longitude
)
{
    return application::Coordinate{
        latitude,
        longitude
    };
}

}


TEST(
    RoutePresentationAggregatorTest,
    AggregatesOrderedSegments
)
{
    application::RoutePresentationAggregator aggregator;


    std::vector<application::RoutePresentationData> segments{
        application::RoutePresentationData{
            {
                point(1.0, 1.0),
                point(2.0, 2.0)
            }
        },

        application::RoutePresentationData{
            {
                point(2.0, 2.0),
                point(3.0, 3.0)
            }
        },

        application::RoutePresentationData{
            {
                point(3.0, 3.0),
                point(4.0, 4.0)
            }
        }
    };


    auto result =
        aggregator.aggregate(segments);


    ASSERT_EQ(
        result.geometryPoints.size(),
        4
    );


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[0].latitude,
        1.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[1].latitude,
        2.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[2].latitude,
        3.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[3].latitude,
        4.0
    );
}


TEST(
    RoutePresentationAggregatorTest,
    DeduplicatesSharedBoundaryPoint
)
{
    application::RoutePresentationAggregator aggregator;


    std::vector<application::RoutePresentationData> segments{
        application::RoutePresentationData{
            {
                point(10.0, 10.0),
                point(20.0, 20.0),
                point(30.0, 30.0)
            }
        },

        application::RoutePresentationData{
            {
                point(30.0, 30.0),
                point(40.0, 40.0),
                point(50.0, 50.0)
            }
        }
    };


    auto result =
        aggregator.aggregate(segments);


    ASSERT_EQ(
        result.geometryPoints.size(),
        5
    );


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[2].latitude,
        30.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[3].latitude,
        40.0
    );
}


TEST(
    RoutePresentationAggregatorTest,
    PreservesExactSegmentOrder
)
{
    application::RoutePresentationAggregator aggregator;


    std::vector<application::RoutePresentationData> segments{
        application::RoutePresentationData{
            {
                point(100.0, 100.0),
                point(200.0, 200.0)
            }
        },

        application::RoutePresentationData{
            {
                point(200.0, 200.0),
                point(50.0, 50.0)
            }
        }
    };


    auto result =
        aggregator.aggregate(segments);


    ASSERT_EQ(
        result.geometryPoints.size(),
        3
    );


    EXPECT_DOUBLE_EQ(
        result.geometryPoints[0].latitude,
        100.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[1].latitude,
        200.0
    );

    EXPECT_DOUBLE_EQ(
        result.geometryPoints[2].latitude,
        50.0
    );
}


TEST(
    RoutePresentationAggregatorTest,
    EmptyInputReturnsEmptyPresentation
)
{
    application::RoutePresentationAggregator aggregator;


    std::vector<application::RoutePresentationData> segments;


    auto result =
        aggregator.aggregate(segments);


    EXPECT_TRUE(
        result.geometryPoints.empty()
    );
}
