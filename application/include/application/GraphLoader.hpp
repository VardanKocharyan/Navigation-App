#pragma once

#include <filesystem>
#include <memory>

#include "core/graph/Graph.hpp"
#include "IGraphImporter.h"

namespace application
{

class GraphLoader
{
public:

    explicit GraphLoader(std::unique_ptr<IGraphImporter> importer);

    void load(const std::filesystem::path& source);

    [[nodiscard]]
    const core::graph::Graph& graph() const;

    [[nodiscard]]
    bool isLoaded() const noexcept;


private:
    std::unique_ptr<IGraphImporter> importer_;
    std::unique_ptr<core::graph::Graph> graph_;
};

}
