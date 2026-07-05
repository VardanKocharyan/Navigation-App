#pragma once

class Graph;

class IGraphSerializer
{
public:
    virtual ~IGraphSerializer() = default;

    // Serialize graph to external format (NOT IMPLEMENTED YET)
    virtual void serialize(const Graph& graph) = 0;

    // Deserialize graph (NOT IMPLEMENTED YET)
    virtual void deserialize(Graph& graph) = 0;
};
