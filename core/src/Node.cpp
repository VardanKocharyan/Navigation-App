#include "core/model/Node.hpp"

namespace core::model {

Node::Node(types::NodeId id)
    : m_id(id) {}

types::NodeId Node::id() const {
    return m_id;
}

} // namespace core::model
