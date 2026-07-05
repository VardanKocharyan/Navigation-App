#pragma once
#include <vector>
#include "../types/Types.hpp"

namespace core::model {

/**
 * @brief Result container for path (NO LOGIC)
 */
class Route {
public:
    using Path = std::vector<types::NodeId>;

    Route() = default;
    explicit Route(Path path);

    const Path& path() const;

private:
    Path m_path;
};

} // namespace core::model
