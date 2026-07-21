#pragma once

#include <filesystem>

#include "ImportedGraph.hpp"


class IGraphImporter
{
public:

    virtual ~IGraphImporter() = default;


    [[nodiscard]]
    virtual persistence::ImportedGraph
    import(
        const std::filesystem::path& file
    ) = 0;
};
