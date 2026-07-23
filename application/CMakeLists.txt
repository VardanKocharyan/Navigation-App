add_library(application
    src/ApplicationTypes.cpp
    src/GraphLoader.cpp
    src/NavigationService.cpp
    src/RoutePresentationBuilder.cpp
    src/RoutePresentationAggregator.cpp
    src/RouteOptimizer.cpp
    src/RuntimeGeometryProvider.cpp
    src/ApplicationBootstrap.cpp
)

target_include_directories(application PUBLIC
    ${CMAKE_SOURCE_DIR}/application/include
)

target_link_libraries(application
    core
    algorithms
    persistence
)
