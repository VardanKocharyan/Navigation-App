#include "core/graph/Graph.hpp"

namespace core::graph {

bool Graph::addNode(const model::Node& node)
{
    const auto id = node.id();

    auto [it, inserted] = nodes_.emplace(id, node);

    if (!inserted) {
        return false; // Duplicate NodeId
    }

    // Ensure an adjacency entry exists for this node.
    adjacency_.try_emplace(id);

    return true;
}

const model::Node* Graph::findNode(types::NodeId id) const
{
    auto it = nodes_.find(id);

    if (it == nodes_.end()) {
        return nullptr;
    }

    return &it->second;
}



} // namespace core::graph
