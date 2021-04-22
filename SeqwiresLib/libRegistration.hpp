#pragma once

namespace babelwires {
    struct ProjectContext;
}

namespace seqwires {
    /// This is not a true plugin model, because everything is statically linked.
    void registerLib(babelwires::ProjectContext& context);
} // namespace seqwires
