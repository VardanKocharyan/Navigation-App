#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "core/graph/Graph.hpp"
#include "core/types/Types.hpp"

#include "IGraphImporter.h"


namespace persistence
{
struct Coordinate;
}


namespace application
{

class GraphLoader
{
public:

    explicit GraphLoader(
        std::unique_ptr<IGraphImporter> importer
    );


    void load(
        const std::filesystem::path& source
    );


    [[nodiscard]]
    const core::graph::Graph&
    graph() const;


    [[nodiscard]]
    const std::unordered_map<
        core::types::NodeId,
        persistence::Coordinate
    >&
    nodeCoordinates() const;


    [[nodiscard]]
    bool isLoaded() const noexcept;


private:

    std::unique_ptr<IGraphImporter>
        importer_;


    std::unique_ptr<
        core::graph::Graph
    >
        graph_;


    std::unique_ptr<
        std::unordered_map<
            core::types::NodeId,
            persistence::Coordinate
        >
    >
        nodeCoordinates_;
};

}
