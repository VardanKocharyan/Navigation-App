#pragma once
#include "../types/Types.hpp"

namespace core::model {

/**
 * @brief Graph node (intersection)
 * PURE DATA ENTITY — no logic
 */
class Node {
public:
    Node() = default;
    Node(types::NodeId id);

    types::NodeId id() const;

private:
    types::NodeId m_id{};
};

} // namespace core::model
