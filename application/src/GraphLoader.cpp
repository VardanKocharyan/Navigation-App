#include "application/GraphLoader.hpp"

#include <stdexcept>


namespace application
{

GraphLoader::GraphLoader(std::unique_ptr<IGraphImporter> importer)
    : importer_(std::move(importer))
{
    if (!importer_)
    {
        throw std::invalid_argument("GraphLoader requires valid IGraphImporter");
    }
}


void GraphLoader::load(const std::filesystem::path& source)
{
    auto loadedGraph = importer_->import(source);


    graph_ = std::make_unique<core::graph::Graph>(std::move(loadedGraph));
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


bool GraphLoader::isLoaded() const noexcept
{
    return graph_ != nullptr;
}

}
