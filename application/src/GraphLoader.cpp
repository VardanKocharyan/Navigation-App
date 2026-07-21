#include "application/GraphLoader.hpp"

#include "ImportedGraph.hpp"

#include <stdexcept>
#include <utility>


namespace application
{

GraphLoader::GraphLoader(
    std::unique_ptr<IGraphImporter> importer
)
    :
    importer_(
        std::move(importer)
    )
{
    if (!importer_)
    {
        throw std::invalid_argument(
            "GraphLoader requires valid IGraphImporter"
        );
    }
}


void
GraphLoader::load(
    const std::filesystem::path& source
)
{
    auto importedGraph =
        importer_->import(
            source
        );


    graph_ =
        std::make_unique<
            core::graph::Graph
        >(
            std::move(
                importedGraph.graph
            )
        );


    nodeCoordinates_ =
        std::make_unique<
            std::unordered_map<
                core::types::NodeId,
                persistence::Coordinate
            >
        >(
            std::move(
                importedGraph.nodeCoordinates
            )
        );
}


const core::graph::Graph&
GraphLoader::graph() const
{
    if (!graph_)
    {
        throw std::runtime_error(
            "Graph has not been loaded"
        );
    }


    return *graph_;
}


const std::unordered_map<
    core::types::NodeId,
    persistence::Coordinate
>&
GraphLoader::nodeCoordinates() const
{
    if (!nodeCoordinates_)
    {
        throw std::runtime_error(
            "Graph has not been loaded"
        );
    }


    return *nodeCoordinates_;
}


bool
GraphLoader::isLoaded() const noexcept
{
    return
        graph_ != nullptr;
}

}
