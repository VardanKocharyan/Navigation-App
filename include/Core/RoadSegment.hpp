#pragma once 
#include <cstdint>

namespace NavApp::Core {

enum class RoadType {
    Highway,
    Primary,
    Secondary,
    Residential
};

enum class RoadStatus {
    Open,
    Close
};

struct RoadSegment {
    uint64_t sourceId{0};
    uint64_t destId{0};
    double distance{0.0};
    bool isOneWay{false};

    RoadType type{RoadType::Residential};
    RoadStatus status{RoadStatus::Open};

    double speedLimit{60.0};
    double trafficFactor{1.0};
};

}


