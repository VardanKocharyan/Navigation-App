#include "GeoJSONImporter.hpp"

#include "internal/GeoJSONParser.hpp"
#include "internal/GraphBuilder.hpp"
#include "internal/RoadFeatureExtractor.hpp"


persistence::ImportedGraph
GeoJSONImporter::import(
    const std::filesystem::path& file
)
{
    persistence::internal::GeoJSONParser parser;

    persistence::internal::RoadFeatureExtractor extractor;

    persistence::internal::GraphBuilder builder;


    const auto parsedFeatures =
        parser.parse(file);


    const auto roadFeatures =
        extractor.extract(
            parsedFeatures
        );


    return builder.build(
        roadFeatures
    );
}
