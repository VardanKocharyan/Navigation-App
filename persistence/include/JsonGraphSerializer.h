#pragma once

#include "IGraphSerializer.h"

class JsonGraphSerializer : public IGraphSerializer
{
public:
    JsonGraphSerializer() = default;
    ~JsonGraphSerializer() override = default;

    void serialize(const Graph& graph) override;

    void deserialize(Graph& graph) override;
};
