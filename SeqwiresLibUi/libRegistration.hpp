#pragma once

namespace babelwires {
    struct UiProjectContext;
}

namespace seqwiresUi {
    /// This is not a true plugin model, because everything is statically linked.
    void registerLib(babelwires::UiProjectContext& context);
} // namespace seqwiresUi
