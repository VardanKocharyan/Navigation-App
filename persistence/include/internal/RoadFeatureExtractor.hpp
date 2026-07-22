#pragma once

#include "GeoJSONParser.hpp"

#include <vector>

namespace persistence::internal {

struct RoadFeature
{
    std::vector<std::vector<Coordinate>> lines;

    bool oneway{false};
};

class RoadFeatureExtractor
{
public:
    std::vector<RoadFeature>
    extract(const std::vector<ParsedFeature>& features) const;

private:
    static bool isSupportedRoad(const std::string& highway);
};

} // namespace persistence::internal
