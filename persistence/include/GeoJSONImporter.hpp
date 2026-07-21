#pragma once

#include <filesystem>

#include "IGraphImporter.h"


class GeoJSONImporter final
    : public IGraphImporter
{
public:

    [[nodiscard]]
    persistence::ImportedGraph
    import(
        const std::filesystem::path& file
    ) override;
};
