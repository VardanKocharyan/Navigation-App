#pragma once

#include <memory>
#include <vector>
#include "unordered_map"

#include "../model/Node.hpp"
#include "../model/Edge.hpp"

namespace core::graph {

class Graph {
public:
    // WRITE OPERATIONS (validated, invariant-safe)
    bool addNode(const model::Node& node);
    bool addEdge(const model::Edge& edge);

    bool removeNode(types::NodeId id);
    bool removeEdge(types::EdgeId id);

    // READ OPERATIONS (safe)
    const model::Node* findNode(types::NodeId id) const;
    const model::Edge* findEdge(types::EdgeId id) const;

    const std::vector<model::Node>& nodes() const noexcept;
    const std::vector<model::Edge>& edges() const noexcept;

    // SAFE ADJACENCY ACCESS
    std::vector<model::Edge>
    getOutgoingEdges(types::NodeId id) const;

    std::vector<types::NodeId>
    getNeighbors(types::NodeId id) const;

private:
    std::unordered_map<types::NodeId, model::Node> nodes_;
    std::unordered_map<types::EdgeId, model::Edge> edges_;
    std::unordered_map<types::NodeId, std::vector<types::EdgeId>> adjacency_;

    mutable std::vector<model::Node> nodesCache_;
    mutable std::vector<model::Edge> edgesCache_;
};

} // namespace core::graph
