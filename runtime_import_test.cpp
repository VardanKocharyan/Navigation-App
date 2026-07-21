#include "GeoJSONImporter.hpp"

#include <iostream>
#include <filesystem>

int main()
{
    GeoJSONImporter importer;

    const std::filesystem::path file =
        "persistence/tests/resources/export.geojson";

    const auto imported =
    importer.import(file);

    std::cout
        << "Nodes: "
        << imported.graph.nodes().size()
        << '\n';

    std::cout
        << "Edges: "
        << imported.graph.edges().size()
        << '\n';

    std::cout
        << "Node coordinates: "
        << imported.nodeCoordinates.size()
        << '\n';

    if (!imported.nodeCoordinates.empty())
    {
        const auto& [nodeId, coordinate] =
            *imported.nodeCoordinates.begin();


        std::cout
            << "Sample NodeId: "
            << nodeId
            << '\n';


        std::cout
            << "Latitude: "
            << coordinate.latitude
            << '\n';


        std::cout
            << "Longitude: "
            << coordinate.longitude
            << '\n';
    }

    return 0;
}
