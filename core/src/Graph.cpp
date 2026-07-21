#include <algorithm>

#include "core/graph/Graph.hpp"

namespace core::graph {

bool Graph::addNode(const model::Node& node)
{
    const auto id = node.id();

    auto [iterator, inserted] = nodes_.emplace(id, node);

    if (!inserted) {
        return false;
    }

    adjacency_.try_emplace(id);

    return true;
}


bool Graph::addEdge(const model::Edge& edge)
{
    const auto edgeId = edge.id();

    if (edges_.contains(edgeId)) {
        return false;
    }

    if (!nodes_.contains(edge.from()) ||
        !nodes_.contains(edge.to())) {
        return false;
    }

    edges_.emplace(edgeId, edge);

    adjacency_[edge.from()].push_back(edgeId);

    return true;
}


bool Graph::removeNode(types::NodeId id)
{
    if (!nodes_.contains(id)) {
        return false;
    }

    std::vector<types::EdgeId> edgesToRemove;

    for (const auto& [edgeId, edge] : edges_) {
        if (edge.from() == id || edge.to() == id) {
            edgesToRemove.push_back(edgeId);
        }
    }

    for (const auto edgeId : edgesToRemove) {
        removeEdge(edgeId);
    }

    adjacency_.erase(id);
    nodes_.erase(id);

    return true;
}


bool Graph::removeEdge(types::EdgeId id)
{
    auto edgeIt = edges_.find(id);

    if (edgeIt == edges_.end()) {
        return false;
    }

    const auto from = edgeIt->second.from();

    auto adjacencyIt = adjacency_.find(from);

    if (adjacencyIt != adjacency_.end()) {

        auto& outgoing = adjacencyIt->second;

        outgoing.erase(
            std::remove(
                outgoing.begin(),
                outgoing.end(),
                id),
            outgoing.end()
        );
    }

    edges_.erase(edgeIt);

    return true;
}


const model::Node* Graph::findNode(types::NodeId id) const
{
    auto iterator = nodes_.find(id);

    if (iterator == nodes_.end()) {
        return nullptr;
    }

    return &iterator->second;
}


const model::Edge* Graph::findEdge(types::EdgeId id) const
{
    auto iterator = edges_.find(id);

    if (iterator == edges_.end()) {
        return nullptr;
    }

    return &iterator->second;
}


const std::vector<model::Node>& Graph::nodes() const noexcept
{
    nodesCache_.clear();
    nodesCache_.reserve(nodes_.size());

    for (const auto& [id, node] : nodes_) {
        nodesCache_.push_back(node);
    }

    return nodesCache_;
}


const std::vector<model::Edge>& Graph::edges() const noexcept
{
    edgesCache_.clear();
    edgesCache_.reserve(edges_.size());

    for (const auto& [id, edge] : edges_) {
        edgesCache_.push_back(edge);
    }

    return edgesCache_;
}


std::vector<model::Edge>
Graph::getOutgoingEdges(types::NodeId id) const
{
    std::vector<model::Edge> result;

    auto adjacencyIt = adjacency_.find(id);

    if (adjacencyIt == adjacency_.end()) {
        return result;
    }

    result.reserve(adjacencyIt->second.size());

    for (const auto edgeId : adjacencyIt->second) {

        auto edgeIt = edges_.find(edgeId);

        if (edgeIt != edges_.end()) {
            result.push_back(edgeIt->second);
        }
    }

    return result;
}


std::vector<types::NodeId>
Graph::getNeighbors(types::NodeId id) const
{
    std::vector<types::NodeId> result;

    auto adjacencyIt = adjacency_.find(id);

    if (adjacencyIt == adjacency_.end()) {
        return result;
    }

    result.reserve(adjacencyIt->second.size());

    for (const auto edgeId : adjacencyIt->second) {

        auto edgeIt = edges_.find(edgeId);

        if (edgeIt != edges_.end()) {
            result.push_back(edgeIt->second.to());
        }
    }

    return result;
}


} // namespace core::graph
