#pragma once

namespace babelwires {
    struct ProjectContext;
}

namespace smf {
    /// This is not a true plugin model, because everything is statically linked.
    void registerLib(babelwires::ProjectContext& context);
} // namespace smf
