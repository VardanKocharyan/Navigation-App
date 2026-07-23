#include <gtest/gtest.h>

#include "application/RouteOptimizer.hpp"

#include <initializer_list>

namespace
{

class FakePathFinder final
    : public algorithms::IPathFinder
{
public:

    struct RouteMapping
    {
        core::types::NodeId source;
        core::types::NodeId destination;

        core::model::Route route;
    };


    std::vector<RouteMapping> routes;


    core::model::Route findPath(
        const core::graph::Graph&,
        core::types::NodeId source,
        core::types::NodeId destination
    ) const override
    {
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

        return core::model::Route{};
    }
};

core::model::Route makeRoute(
    std::initializer_list<core::types::NodeId> nodes
)
{
    return core::model::Route(
        core::model::Route::Path(
            nodes
        )
    );
}

}


TEST(
    RouteOptimizerTest,
    EmptyDestinationListReturnsFailure
)
{
    core::graph::Graph graph;


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    application::DestinationList destinations;


    auto result =
        optimizer.optimize(
            1,
            destinations,
            application::OptimizationStrategy::Exact
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_TRUE(
        result.orderedDestinations.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->message,
        "No destinations provided"
    );


    EXPECT_FALSE(
        result.failure->affectedNode.has_value()
    );
}

TEST(
    RouteOptimizerTest,
    SingleDestinationReturnsIdentityResult
)
{
    core::graph::Graph graph;

    const auto start = 1;
    const auto destination = 2;

    graph.addNode(
        core::model::Node(start)
    );

    graph.addNode(
        core::model::Node(destination)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        destination
    };


    const auto result =
        optimizer.optimize(
            start,
            destinations,
            application::OptimizationStrategy::Exact
        );


    ASSERT_TRUE(
        result.success
    );


    ASSERT_FALSE(
        result.failure.has_value()
    );


    ASSERT_EQ(
        result.orderedDestinations.size(),
        1
    );


    EXPECT_EQ(
        result.orderedDestinations[0],
        destination
    );
}


    TEST(
    RouteOptimizerTest,
    InvalidStartNodeReturnsFailure
)
{
    core::graph::Graph graph;

    const auto invalidStart = 999;
    const auto destination = 2;

    graph.addNode(
        core::model::Node(destination)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        destination
    };


    const auto result =
        optimizer.optimize(
            invalidStart,
            destinations,
            application::OptimizationStrategy::Exact
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_TRUE(
        result.orderedDestinations.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->affectedNode,
        invalidStart
    );
}

TEST(
    RouteOptimizerTest,
    InvalidDestinationNodeReturnsFailure
)
{
    core::graph::Graph graph;

    const auto start = 1;
    const auto invalidDestination = 999;

    graph.addNode(
        core::model::Node(start)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        invalidDestination
    };


    const auto result =
        optimizer.optimize(
            start,
            destinations,
            application::OptimizationStrategy::Exact
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_TRUE(
        result.orderedDestinations.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->affectedNode,
        invalidDestination
    );
}

TEST(
    RouteOptimizerTest,
    DuplicateDestinationsReturnFailure
)
{
    core::graph::Graph graph;

    const auto start = 1;
    const auto destinationA = 2;
    const auto destinationB = 3;


    graph.addNode(
        core::model::Node(start)
    );


    graph.addNode(
        core::model::Node(destinationA)
    );


    graph.addNode(
        core::model::Node(destinationB)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        destinationA,
        destinationB,
        destinationA
    };


    const auto result =
        optimizer.optimize(
            start,
            destinations,
            application::OptimizationStrategy::Exact
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_TRUE(
        result.orderedDestinations.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );


    EXPECT_EQ(
        result.failure->message,
        "Duplicate destination"
    );


    ASSERT_TRUE(
        result.failure->affectedNode.has_value()
    );


    EXPECT_EQ(
        result.failure->affectedNode.value(),
        destinationA
    );
}

TEST(
    RouteOptimizerTest,
    ExactStrategyChoosesCheapestOrder
)
{
    core::graph::Graph graph;

    const auto start = 1;
    const auto destinationA = 2;
    const auto destinationB = 3;


    graph.addNode(
        core::model::Node(start)
    );

    graph.addNode(
        core::model::Node(destinationA)
    );

    graph.addNode(
        core::model::Node(destinationB)
    );

    graph.addEdge(
        core::model::Edge(1, 1, 2, 10.0)
    );

    graph.addEdge(
        core::model::Edge(2, 1, 3, 5.0)
    );

    graph.addEdge(
        core::model::Edge(3, 2, 3, 100.0)
    );

    graph.addEdge(
        core::model::Edge(4, 3, 2, 5.0)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    pathFinder->routes =
    {
        {
            start,
            destinationA,
            makeRoute({1, 2})
        },

        {
            start,
            destinationB,
            makeRoute({1, 3})
        },

        {
            destinationA,
            destinationB,
            makeRoute({2, 3})
        },

        {
            destinationB,
            destinationA,
            makeRoute({3, 2})
        }
    };


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        destinationA,
        destinationB
    };


    const auto result =
        optimizer.optimize(
            start,
            destinations,
            application::OptimizationStrategy::Exact
        );


    ASSERT_TRUE(
        result.success
    );


    ASSERT_EQ(
        result.orderedDestinations.size(),
        2
    );


    EXPECT_EQ(
        result.orderedDestinations[0],
        destinationB
    );

    EXPECT_EQ(
        result.orderedDestinations[1],
        destinationA
    );
}

TEST(
    RouteOptimizerTest,
    UnreachableDestinationReturnsFailure
)
{
    core::graph::Graph graph;

    const auto start = 1;
    const auto destinationA = 2;
    const auto destinationB = 3;


    graph.addNode(
        core::model::Node(start)
    );

    graph.addNode(
        core::model::Node(destinationA)
    );

    graph.addNode(
        core::model::Node(destinationB)
    );


    auto pathFinder =
        std::make_unique<FakePathFinder>();


    pathFinder->routes =
    {
        {
            start,
            destinationA,
            makeRoute({1, 2})
        }

        // destinationA → destinationB
        // intentionally missing:
        //
        // no valid route
    };


    application::RouteOptimizer optimizer(
        graph,
        std::move(pathFinder)
    );


    const application::DestinationList destinations{
        destinationA,
        destinationB
    };


    const auto result =
        optimizer.optimize(
            start,
            destinations,
            application::OptimizationStrategy::Exact
        );


    EXPECT_FALSE(
        result.success
    );


    EXPECT_TRUE(
        result.orderedDestinations.empty()
    );


    ASSERT_TRUE(
        result.failure.has_value()
    );
}
