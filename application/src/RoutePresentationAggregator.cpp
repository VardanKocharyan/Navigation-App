#include "application/RoutePresentationAggregator.hpp"


namespace application
{


RoutePresentationData
RoutePresentationAggregator::aggregate(
    const std::vector<RoutePresentationData>& segments
) const
{
    RoutePresentationData result;


    for (const auto& segment : segments)
    {
        if (segment.geometryPoints.empty())
        {
            continue;
        }


        if (result.geometryPoints.empty())
        {
            result.geometryPoints =
                segment.geometryPoints;

            continue;
        }


        std::size_t startIndex = 0;


        if (
            result.geometryPoints.back().latitude
                == segment.geometryPoints.front().latitude
            &&
            result.geometryPoints.back().longitude
                == segment.geometryPoints.front().longitude
        )
        {
            startIndex = 1;
        }


        result.geometryPoints.insert(
            result.geometryPoints.end(),
            segment.geometryPoints.begin() + startIndex,
            segment.geometryPoints.end()
        );
    }


    return result;
}


}
