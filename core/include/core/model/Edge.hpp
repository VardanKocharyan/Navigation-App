#pragma once
#include "../types/Types.hpp"

namespace core::model {

/**
 * @brief Directed connection between nodes
 * PURE DATA ENTITY
 */
class Edge {
public:
    Edge() = default;

    Edge(types::EdgeId id,
         types::NodeId from,
         types::NodeId to,
         types::Weight weight);

    types::EdgeId id() const;
    types::NodeId from() const;
    types::NodeId to() const;
    types::Weight weight() const;

private:
    types::EdgeId m_id{};
    types::NodeId m_from{};
    types::NodeId m_to{};
    types::Weight m_weight{};
};

} // namespace core::model
