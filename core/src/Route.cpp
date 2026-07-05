#include "core/model/Route.hpp"

namespace core::model {

Route::Route(Path path)
    : m_path(std::move(path)) {}

const Route::Path& Route::path() const {
    return m_path;
}

} // namespace core::model
