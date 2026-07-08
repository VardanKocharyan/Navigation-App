#include "application/GraphLoader.hpp"
#include "GeoJSONImporter.h"

#include <cassert>


int main()
{
    auto importer =
        std::make_unique<GeoJSONImporter>();


    application::GraphLoader loader(
        std::move(importer)
    );


    loader.load(
        "resources/export.geojson"
    );


    assert(loader.isLoaded());


    const auto& graph =
        loader.graph();


    assert(!graph.nodes().empty());
    assert(!graph.edges().empty());


    return 0;
}
