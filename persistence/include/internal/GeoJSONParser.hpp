#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace persistence::internal {

struct Coordinate
{
    double longitude{};
    double latitude{};
};

enum class GeometryType
{
    LineString,
    MultiLineString,
    Unsupported
};

struct ParsedFeature
{
    std::string highway;

    bool oneway{false};

    GeometryType geometryType{
        GeometryType::Unsupported
    };

    std::vector<
        std::vector<Coordinate>
    >
    lines;
};

class GeoJSONParser
{
public:
    std::vector<ParsedFeature>
    parse(const std::filesystem::path& file) const;
};

} // namespace persistence::internal
