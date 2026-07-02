#include "../include/Core/Graph.hpp"

using G = NavApp::Core::Graph;

void G::addIntersection(const Intersection& intersection) {
    if (!intersection.isValid()) return;
    m_vertices[intersection.id] = intersection;
}

void G::addRoadSegment(const RoadSegment& roadSegment) {
    if (!m_adjacencyList.contains(roadSegment.sourceId) || !m_adjacencyList.contains(destId)) return;
    
    m_adjacencyList[roadSegment.sourceId].push_back(roadSegment);

    if (!roadSegment.isOneWay){
        RoadSegment tmp = roadSegment;
        std::swap(tmp.sourceId, tmp.destId);
        m_adjacencyList[roadSegment.destId].push_back(tmp);
    }
}

void G::clear() {
    m_vertices.clear();
    m_adjacencyList.clear();
}

const NavApp::Core::Intersection* G::getIntersection(uint64_t id) const {
    auto it = m_adjacencyList.find(id);
    if (it != m_vertices.end()) {
        return &(it->second);
    }
    return nullptr;
}

const std::vector<RoadSegment>& G::getOutgoingEdges(uint64_t vertexId) const {
    auto it = m_adjacencyList.find(vertexId);
    if (it != m_adjacencyList.end()) {
        return it->second;
    }
    return m_emptyEdges;
}

const std::unordered_map<uint64_t, Intersection>& G::getVertices() const {
    return m_vertices;
}

