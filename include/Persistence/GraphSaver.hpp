#pragma once 
#include <QString>

namespace NavApp::Core { class Graph; }

namespace NavApp::Persistence {
struct GraphSaver {
    static bool saveToFile(const QString& filePath, const Core::Graph& graph); // return true if all is ok 
};

}
