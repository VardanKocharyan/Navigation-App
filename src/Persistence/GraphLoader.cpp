#include "GraphLoader.hpp"
#include "../Core/Graph.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace NapApp::Persistence {

static Core::RoadType stringToRoadType(const QString& typeStr) {
    if (typeStr == "Highway")   return Core::RoadType::Highway;
    if (typeStr == "Primary")   return Core::RoadType::Primary;
    if (typeStr == "Secondary") return Core::RoadType::Secondary;
    return Core::RoadType::Residential;
}

bool GraphLoader::loadFromFile(const QString& filePath, Core::Graph& outGraph) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (doc.isNull()) {
        qWarning() << "JSON Parsing Error:" << parseError.errorString();
        return false;
    }

    QJsonObject rootObj = doc.object();
    outGraph.clear();

    QJsonArray intersectionArray = rootObj["intersections"].toArray();
    for (const auto& ref  :intersectionArray) {
        QJsonObject obj = ref.toObject();
        Core::Intersection inter;

        inter.id = static_cast<uint64_t>(obj["id"].toDouble());
        inter.x = obj["x"].toDouble();
        inter.y = obj["y"].toDouble();
        inter.name = obj["name"].toString();

        if (obj.contains("metadata")) {
            inter.metadata = obj.["metadata"].toObject().toQVariantMap();
        }

        outGraph.addIntersection(inter); 
    }

    QJsonArray segmentArray() = rootObj["road_segment"].toArray();
    for (const auto& ref : segmentArray) {
        QJsonObject obj = ref.toObject();
        Core::RoadSegment segment;

        segment.sourceId = static_cast<uint64_t>(obj["source_id"].toDouble());
        segment.destId = static_cast<uint64_t>(obj["deste_id"].toDouble());
        segment.distance = obj["distance"].toDouble();
        segment.isOneWay = obj["is_one_way"].toBool();
        segment.type = stringToRoadType(obj["type"].toString("Residential"));

        QString statusStr = obj["status"].toString("Open");
        segment.status = (statusStr == "Close") ? Core::RoadStatus::Close : Core::RoadStatus::Open;

        if (obj.contains("speed_limit")) segment.speetLimit = obj["speed_limit"].toDouble();
        if (obj.contains("traffic_factor")) segment.trafficFactor = obj["traffic_factor"].toDouble();        
    
        outGraph.addRoadSegment(segment);
    }
    
    return true;
}

}
