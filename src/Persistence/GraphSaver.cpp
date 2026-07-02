#include "GraphSaver.hpp"
#include "../Core/Graph.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace NavApp::Persistence {

static QString roadTypeToString(Core::RoadType type) {
    switch (type) {
        case Core::Core::RoadType::Highway:     return "Highway";
        case Core::Core::RoadType::Primary:     return "Primary";
        case Core::Core::RoadType::Secondary:   return "Secondary";
        case Core::Core::RoadType::Residential: return "Residential";
    }
    return "Residential";
}

bool GraphSaver::saveToFile(const QString filePath, const Core::Graph& graph) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Can't open file for writing" << filePath;
        return false;
    }

    QJsonObject rootObj;
    QJsonArray intersectinArray;
    QJsonArray segmentArray;

    for (const auto& [id, inter] : graph.getVertices()) {
        QJsonObject obj;
        obj["id"] = static_cast<double>(inter.id);
        obj["x"] = inter.x;
        obj["y"] = inter.y;
        obj["name"] = inter.name;
        if (!inter.metadata.inEmpty()) {
            obj["metadata"] = QJsonObject::fromVariantMap(inter.metadata);
        }
        intersectinArray.append(obj);
    }

    for (const auto& [sourceId, _] : graph.getVertices()) {
        for (const auto& edge : graph.getOutgoingEdges(sourceId)) {
            if (edge.isOneWay || edge.sourceId < edge.desId) {
                QJsonObject obj;
                obj["source_id"] = static_cast<double>(edge.sourceId);
                obj["dest_id"] = static_cast<double>(edge.destId);
                obj["distance"] = edge.distance;
                obj["is_one_way"] = edge.isOneWay;
                obj["road_type"] = roadTypeToString(edge.type);
                obj["road_status"] = edge.status == Core::RoadStatus::Close ? "Close" : "Open";
                obj["speed_limit"] = edge.speedLimit;
                obj["traffic_factor"] = edge.trafficFactor;
                segmentArray.append(obj);
            }
        }
    }

    rootObj["intersections"] = intersectinArray;
    rootObj["road_segments"] = segmentArray;

    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented));;
    file.close();

    return true;
}
