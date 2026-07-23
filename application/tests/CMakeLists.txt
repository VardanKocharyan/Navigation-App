find_package(GTest REQUIRED)

add_executable(
    application_tests
    test_navigation_service.cpp
    test_route_presentation_builder.cpp
    test_route_presentation_aggregator.cpp
    test_route_optimizer.cpp
)

target_link_libraries(application_tests
    PRIVATE
    application
    core
    algorithms
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(application_tests)
