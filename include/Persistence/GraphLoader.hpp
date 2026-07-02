#pragma once
#include <QString>

namespace NavApp::Core { class Graph; }

namespace NavApp::Persistence {

struct GraphLoader {
    static bool loadFromFile(const QString& filePath, Core::Graph& outGraph); // true if we can read file 
};

}
