#include <gtest/gtest.h>

#include <memory>

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



class FakePathFinder final
    : public IPathFinder
{

public:

    mutable int calls{0};

    core::model::Route routeToReturn;


    core::model::Route findPath(
        const core::graph::Graph&,
        core::types::NodeId,
        core::types::NodeId
    ) const override
    {
        ++calls;

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
