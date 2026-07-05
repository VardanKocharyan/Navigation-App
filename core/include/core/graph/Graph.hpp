#pragma once

#include <memory>

#include "../model/Node.hpp"
#include "../model/Edge.hpp"

namespace core::graph {

/**
 * @brief Core graph structure
 * PURE INTERFACE LAYER (NO LOGIC)
 */
class Graph {
public:
    Graph() = default;
    ~Graph() = default;

    // Disable copying for safety
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    // ---- API (EMPTY IMPLEMENTATION LATER) ----

    void addNode(const model::Node& node);
    void addEdge(const model::Edge& edge);

    void removeNode(types::NodeId id);
    void removeEdge(types::EdgeId id);
};

} // namespace core::graph
