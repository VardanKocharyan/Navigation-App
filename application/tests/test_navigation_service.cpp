#include <gtest/gtest.h>

#include <memory>

#include "Interfaces/IPathFinder.hpp"

#include "application/NavigationService.hpp"
#include "application/INodeGeometryProvider.hpp"

#include "core/model/Node.hpp"
#include "core/model/Route.hpp"


using namespace application;


class FakeGeometryProvider final
    : public INodeGeometryProvider
{

public:

    std::optional<Coordinate> getCoordinate(
        core::types::NodeId nodeId
    ) const override
    {
        return Coordinate{
            static_cast<double>(nodeId),
            static_cast<double>(nodeId)
        };
    }

};

struct Call
{
    core::types::NodeId source;

    core::types::NodeId destination;
};

class FakePathFinder final
    : public algorithms::IPathFinder
{
public:

    struct Call
    {
        core::types::NodeId source;
        core::types::NodeId destination;
    };


    struct RouteMapping
    {
        core::types::NodeId source;
        core::types::NodeId destination;

        core::model::Route route;
    };


    mutable int calls{0};


    mutable std::vector<Call> callHistory;


    core::model::Route routeToReturn;


    std::vector<RouteMapping> routes;


    core::model::Route findPath(
        const core::graph::Graph&,
        core::types::NodeId source,
        core::types::NodeId destination
    ) const override
    {
        ++calls;


        callHistory.push_back(
            Call{
                source,
                destination
            }
        );


        for (const auto& mapping : routes)
        {
            if (
                mapping.source == source
                &&
                mapping.destination == destination
            )
            {
                return mapping.route;
            }
        }


        return routeToReturn;
    }
};

static core::graph::Graph createTestGraph()
{
    core::graph::Graph graph;


    graph.addNode(
        core::model::Node(1)
    );

    graph.addNode(
        core::model::Node(2)
    );

    graph.addNode(
        core::model::Node(3)
    );


    return graph;
}


TEST(
    NavigationServiceTest,
    MultiDestinationRouteBuildsAggregatedPresentation
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    pathFinder->routes.push_back(
        {
            1,
            2,
            core::model::Route(
                {
                    1,
                    2
                }
            )
        }
    );


    pathFinder->routes.push_back(
        {
            2,
            3,
            core::model::Route(
                {
                    2,
                    3
                }
            )
        }
    );


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        3
    };


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_TRUE(
        result.success
    );


    ASSERT_TRUE(
        result.route.has_value()
    );


    ASSERT_EQ(
        result.route->geometryPoints.size(),
        3
    );


    EXPECT_DOUBLE_EQ(
        result.route->geometryPoints[0].latitude,
        1.0
    );


    EXPECT_DOUBLE_EQ(
        result.route->geometryPoints[1].latitude,
        2.0
    );


    EXPECT_DOUBLE_EQ(
        result.route->geometryPoints[2].latitude,
        3.0
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        2
    );
}


TEST(
    NavigationServiceTest,
    MultiDestinationRoutePreservesOrder
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    core::model::Route::Path path{
        1,
        2,
        3
    };


    pathFinder->routeToReturn =
        core::model::Route(path);


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        3
    };


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_TRUE(
        result.success
    );


    ASSERT_EQ(
        result.segments.size(),
        2
    );


    ASSERT_EQ(
        pathFinderPtr->callHistory.size(),
        2
    );


    EXPECT_EQ(
        pathFinderPtr->callHistory[0].source,
        1
    );


    EXPECT_EQ(
        pathFinderPtr->callHistory[0].destination,
        2
    );


    EXPECT_EQ(
        pathFinderPtr->callHistory[1].source,
        2
    );


    EXPECT_EQ(
        pathFinderPtr->callHistory[1].destination,
        3
    );
}

TEST(
    NavigationServiceTest,
    EmptyDestinationListFails
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations;


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_EQ(
        result.message,
        "No destinations provided"
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        0
    );
}

TEST(
    NavigationServiceTest,
    MultiDestinationInvalidStartFails
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        3
    };


    auto result =
        service.requestMultiDestinationRoute(
            99,
            destinations
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_EQ(
        result.message,
        "Invalid start node"
    );


    EXPECT_FALSE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        0
    );
}


TEST(
    NavigationServiceTest,
    ValidRouteReturnsSuccessfulResult
)
{
    auto graph = createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    core::model::Route::Path path{
        1,
        2,
        3
    };


    pathFinder->routeToReturn =
        core::model::Route(path);


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    auto result =
        service.calculateRoute(
            1,
            3
        );


    EXPECT_TRUE(result.success);

    EXPECT_TRUE(
        result.route.has_value()
    );

    EXPECT_FALSE(
        result.route->geometryPoints.empty()
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        1
    );
}

TEST(
    NavigationServiceTest,
    InvalidDestinationFailsAtCorrectSegment
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    core::model::Route::Path path{
        1,
        2
    };


    pathFinder->routeToReturn =
        core::model::Route(path);


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        99,
        3
    };


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_FALSE(
        result.success
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->segmentIndex,
        1
    );


    EXPECT_EQ(
        result.failure->source,
        2
    );


    EXPECT_EQ(
        result.failure->destination,
        99
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        1
    );


    EXPECT_EQ(
        result.segments.size(),
        1
    );
}


TEST(
    NavigationServiceTest,
    ConsecutiveIdenticalDestinationsFail
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();

    core::model::Route::Path path{
        1,
        2
    };

    pathFinder->routeToReturn =
        core::model::Route(path);

    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        2
    };


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_FALSE(
        result.success
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->segmentIndex,
        1
    );


    EXPECT_EQ(
        result.failure->source,
        2
    );


    EXPECT_EQ(
        result.failure->destination,
        2
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        1
    );
}

TEST(
    NavigationServiceTest,
    MultiDestinationFailureIsFailFast
)
{
    auto graph =
        createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    pathFinder->routeToReturn =
        core::model::Route();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    DestinationList destinations{
        2,
        3
    };


    auto result =
        service.requestMultiDestinationRoute(
            1,
            destinations
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        1
    );


    EXPECT_TRUE(
        result.segments.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->segmentIndex,
        0
    );
}



TEST(
    NavigationServiceTest,
    InvalidStartNodeDoesNotCallPathFinder
)
{
    auto graph = createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    auto result =
        service.calculateRoute(
            99,
            3
        );


    EXPECT_FALSE(result.success);


    EXPECT_EQ(
        result.message,
        "Invalid start node"
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        0
    );
}



TEST(
    NavigationServiceTest,
    InvalidDestinationNodeDoesNotCallPathFinder
)
{
    auto graph = createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    auto result =
        service.calculateRoute(
            1,
            99
        );


    EXPECT_FALSE(result.success);


    EXPECT_EQ(
        result.message,
        "Invalid destination node"
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        0
    );
}



TEST(
    NavigationServiceTest,
    EmptyRouteReturnsUnavailable
)
{
    auto graph = createTestGraph();


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    auto* pathFinderPtr =
        pathFinder.get();


    pathFinder->routeToReturn =
        core::model::Route();


    FakeGeometryProvider geometryProvider;


    NavigationService service(
        graph,
        std::move(pathFinder),
        geometryProvider
    );


    auto result =
        service.calculateRoute(
            1,
            3
        );


    EXPECT_FALSE(result.success);


    EXPECT_EQ(
        result.message,
        "Route unavailable"
    );


    EXPECT_EQ(
        pathFinderPtr->calls,
        1
    );
}
