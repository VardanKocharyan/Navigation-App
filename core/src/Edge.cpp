#include "core/model/Edge.hpp"

namespace core::model {

Edge::Edge(types::EdgeId id,
           types::NodeId from,
           types::NodeId to,
           types::Weight weight)
    : m_id(id),
      m_from(from),
      m_to(to),
      m_weight(weight) {}

types::EdgeId Edge::id() const { return m_id; }
types::NodeId Edge::from() const { return m_from; }
types::NodeId Edge::to() const { return m_to; }
types::Weight Edge::weight() const { return m_weight; }

} // namespace core::model
