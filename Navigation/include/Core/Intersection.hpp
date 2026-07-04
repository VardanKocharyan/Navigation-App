#pragma once
#include <QString>
#include <QVariantMap>
#include <cstdint>

namespace NavApp::Core {
    struct Intersection {
        uint64_t id;
        double x{0.0};
        double y{0.0};

        bool isValid() {return id != 0; }
        bool operator==(const Intersection&) const = default;
    };
}
