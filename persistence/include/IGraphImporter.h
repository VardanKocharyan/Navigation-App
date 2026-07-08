#pragma once 

#include "core/graph/Graph.hpp"

class IGraphImporter
{
public:
    virtual ~IGraphImporter() = default;

    [[nodiscard]]
    virtual core::graph::Graph import(const std::filesystem::path& file) = 0;
};
