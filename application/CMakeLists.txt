add_library(application
    src/NavigationService.cpp
    src/GraphLoader.cpp
    src/ApplicationTypes.cpp
    src/RoutePresentationBuilder.cpp
)

target_include_directories(application PUBLIC
    ${CMAKE_SOURCE_DIR}/application/include
)

target_link_libraries(application
    core
    algorithms
    persistence
)
