#include "internal/RoadFeatureExtractor.hpp"

#include <cmath>

namespace persistence::internal
{

std::vector<RoadFeature>
RoadFeatureExtractor::extract(
    const std::vector<ParsedFeature>& features) const
{
    std::vector<RoadFeature> roads;

    roads.reserve(features.size());

    for (const auto& feature : features)
    {
        if (!isSupportedRoad(feature.highway))
        {
            continue;
        }

        if (feature.geometryType == GeometryType::Unsupported)
        {
            continue;
        }

        if (feature.lines.empty())
        {
            continue;
        }

        bool valid = true;

        for (const auto& line : feature.lines)
        {
            if (line.size() < 2)
            {
                valid = false;
                break;
            }

            for (const auto& coordinate : line)
            {
                if (!std::isfinite(coordinate.longitude) ||
                    !std::isfinite(coordinate.latitude))
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
            {
                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        roads.push_back(RoadFeature{
            feature.lines
        });
    }

    return roads;
}


bool
RoadFeatureExtractor::isSupportedRoad(
    const std::string& highway)
{
    return highway == "motorway" ||
           highway == "trunk" ||
           highway == "primary" ||
           highway == "secondary" ||
           highway == "tertiary" ||
           highway == "residential" ||
           highway == "unclassified" ||
           highway == "service" ||
           highway == "living_street";
}

} // namespace persistence::internal
